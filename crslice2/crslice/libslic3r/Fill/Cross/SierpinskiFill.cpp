// Copyright (c) 2022 Ultimaker B.V.
// CuraEngine is released under the terms of the AGPLv3 or higher

#include "SierpinskiFill.h"

#include <algorithm> // swap
#include <assert.h>
#include <functional> // function
#include <iterator> // next, prev

#include "ccglobal/log.h"

//#include "infill/ImageBasedDensityProvider.h"
//#include "infill/UniformDensityProvider.h"
//#include "utils/AABB3D.h"
//#include "utils/polygon.h"

//#include "tools/SVG.h"



namespace Slic3r::Cross
{
#define INT2MM(n) (static_cast<double>(n) / 1000.0)
#define INT2MM2(n) (static_cast<double>(n) / 1000000.0)
static constexpr float sqrt2 = 1.41421356237f;

    static inline coord_t vSize2(const Point& p0)
	{
		return p0.x() * p0.x() + p0.y() * p0.y();
	}

	static inline coord_t vSize(const Point& p0)
	{
		return sqrt(vSize2(p0));
	}
	static inline double vSizeMM(const Point& p0)
	{
		double fx = INT2MM(p0.x());
		double fy = INT2MM(p0.y());
		return sqrt(fx * fx + fy * fy);
	}
	static inline Point normal(const Point& p0, coord_t len)
	{
		coord_t _len = vSize(p0);
		if (_len < 1)
			return Point(len, 0);
		return p0 * len / _len;
	}
	static inline coord_t dot(const Point& p0, const Point& p1)
	{
		return p0.x() * p1.x() + p0.y() * p1.y();
	}

    static constexpr bool diagonal = true;
    static constexpr bool straight = false;

    static constexpr float allowed_length_error = .01;

    static constexpr bool deep_debug_checking = false;

    /*!
 * Parent class of function objects which return the density required for a given region.
 *
 * This density requirement can be based on user input, distance to the 3d model shell, Z distance to top skin, etc.
 */
    class DensityProvider
    {
    public:
        /*!
         * \return the approximate required density of a cube
         */
        virtual float operator()(const BoundingBox3& aabb) const = 0;
        virtual ~DensityProvider()
        {
        };
    };

    SierpinskiFill::SierpinskiFill(const DensityProvider& density_provider, const BoundingBox aabb, int max_depth, const coord_t line_width, bool dithering)
        : dithering(dithering)
        , constraint_error_diffusion(dithering)
        , density_provider(density_provider)
        , aabb(aabb)
        , line_width(line_width)
        , max_depth(max_depth)
    {
        createTree();

        createLowerBoundSequence();

        for (SierpinskiTriangle* node : sequence)
        {
            if (node->getValueError() < -allowed_length_error)
            {
                LOGE("Node is subdivided without the appropriate value! value_error: {} from base {} el: {} er: {}, while the realized_length = {}",
                    node->getValueError(),
                    node->requested_length,
                    node->error_left,
                    node->error_right,
                    node->realized_length);
                assert(false);
            }
        }
        debugCheck(true);

        settleErrors();

        diffuseError();
    }

    SierpinskiFill::~SierpinskiFill()
    {
    }


    void SierpinskiFill::createTree()
    {
        Point lt = Point(aabb.min(0), aabb.max(1));
        Point rb = Point(aabb.max(0), aabb.min(1));

        bool root_straight_corner_is_left = false;
        int root_depth = 1;
        root.children.emplace_back(rb, aabb.min, aabb.max, SierpinskiTriangle::SierpinskiDirection::AC_TO_AB, root_straight_corner_is_left, root_depth);
        root.children.emplace_back(lt, aabb.max, aabb.min, SierpinskiTriangle::SierpinskiDirection::AC_TO_AB, root_straight_corner_is_left, root_depth);
        for (SierpinskiTriangle& triangle : root.children)
        {
            createTree(triangle);
        }

        // calculate node statistics
        createTreeStatistics(root);

        createTreeRequestedLengths(root);
    }

    void SierpinskiFill::createTree(SierpinskiTriangle& sub_root)
    {
        if (sub_root.depth < max_depth) // We need to subdivide.
        {
            SierpinskiTriangle& t = sub_root;
            Point middle = (t.a + t.b) / 2;
            // At each subdivision we divide the triangle in two.
            // Figure out which sort of triangle each child will be:
            SierpinskiTriangle::SierpinskiDirection first_dir, second_dir;
            switch (t.dir)
            {
            default:
            case SierpinskiTriangle::SierpinskiDirection::AB_TO_BC:
                first_dir = SierpinskiTriangle::SierpinskiDirection::AC_TO_BC;
                second_dir = SierpinskiTriangle::SierpinskiDirection::AC_TO_AB;
                break;
            case SierpinskiTriangle::SierpinskiDirection::AC_TO_AB:
                first_dir = SierpinskiTriangle::SierpinskiDirection::AB_TO_BC;
                second_dir = SierpinskiTriangle::SierpinskiDirection::AC_TO_BC;
                break;
            case SierpinskiTriangle::SierpinskiDirection::AC_TO_BC:
                first_dir = SierpinskiTriangle::SierpinskiDirection::AB_TO_BC;
                second_dir = SierpinskiTriangle::SierpinskiDirection::AC_TO_AB;
                break;
            }
            sub_root.children.emplace_back(middle, t.a, t.straight_corner, first_dir, !t.straight_corner_is_left, t.depth + 1);
            sub_root.children.emplace_back(middle, t.straight_corner, t.b, second_dir, !t.straight_corner_is_left, t.depth + 1);
            for (SierpinskiTriangle& child : t.children)
            {
                createTree(child);
            }
        }
    }
    void SierpinskiFill::createTreeStatistics(SierpinskiTriangle& triangle)
    {
        Point ac = triangle.straight_corner - triangle.a;
        float area = 0.5 * INT2MM2(vSize2(ac));
        float short_length = .5 * vSizeMM(ac);
        float long_length = .5 * vSizeMM(triangle.b - triangle.a);
        triangle.area = area;
        triangle.realized_length = (triangle.dir == SierpinskiTriangle::SierpinskiDirection::AC_TO_BC) ? long_length : short_length;
        for (SierpinskiTriangle& child : triangle.children)
        {
            createTreeStatistics(child);
        }
    }


    void SierpinskiFill::createTreeRequestedLengths(SierpinskiTriangle& triangle)
    {
        if (triangle.children.empty())
        { // set requested_length of leaves
            BoundingBox triangle_aabb;
            triangle_aabb.merge(triangle.a);
            triangle_aabb.merge(triangle.b);
            triangle_aabb.merge(triangle.straight_corner);
            BoundingBox3 triangle_aabb3d(Vec3crd(triangle_aabb.min(0), triangle_aabb.min(1), 0), Vec3crd(triangle_aabb.max(0), triangle_aabb.max(1), 1));
            float density = density_provider(triangle_aabb3d); // The density of the square around the triangle is a rough estimate of the density of the triangle.
            triangle.requested_length = density * triangle.area / INT2MM(line_width);
        }
        else
        { // bubble total up requested_length and total_child_realized_length
            for (SierpinskiTriangle& child : triangle.children)
            {
                createTreeRequestedLengths(child);

                triangle.requested_length += child.requested_length;
                triangle.total_child_realized_length += child.realized_length;
            }
        }
    }

    void SierpinskiFill::createLowerBoundSequence()
    {
        sequence.emplace_front(&root);

        if (deep_debug_checking)
            debugCheck();
        for (int iteration = 0; iteration < 999; iteration++)
        {
            bool change = false;
            change |= subdivideAll();
            if (deep_debug_checking)
                debugCheck();

            if (constraint_error_diffusion)
            {
                change |= bubbleUpConstraintErrors();
                if (deep_debug_checking)
                    debugCheck();
            }

            if (!change)
            {
                LOGD("Finished after {} iterations, with a max depth of {}.", iteration + 1, max_depth);
                break;
            }
        }
    }

    std::vector<std::vector<std::list<SierpinskiFill::SierpinskiTriangle*>::iterator>> SierpinskiFill::getDepthOrdered()
    {
        std::vector<std::vector<std::list<SierpinskiTriangle*>::iterator>> depth_ordered(max_depth + 1);
        depth_ordered.resize(max_depth);
        for (std::list<SierpinskiTriangle*>::iterator it = sequence.begin(); it != sequence.end(); ++it)
        {
            SierpinskiTriangle* node = *it;
            depth_ordered[node->depth].emplace_back(it);
        }
        return depth_ordered;
    }

    bool SierpinskiFill::subdivideAll()
    {
        std::vector<std::vector<std::list<SierpinskiTriangle*>::iterator>> depth_ordered = getDepthOrdered();

        bool change = false;
        for (std::vector<std::list<SierpinskiTriangle*>::iterator>& depth_nodes : depth_ordered)
            for (std::list<SierpinskiTriangle*>::iterator it : depth_nodes)
            {
                SierpinskiTriangle* node = *it;
                SierpinskiTriangle& triangle = *node;

                // The range of consecutive triangles to consider for subdivision simultaneously.
                // Two triangles connected to each other via the long edge must be subdivided simultaneously,
                // so then the range will be two long rather than one.
                std::list<SierpinskiTriangle*>::iterator begin = it;
                std::list<SierpinskiTriangle*>::iterator end = std::next(it);
                if (triangle.dir == SierpinskiTriangle::SierpinskiDirection::AC_TO_AB && end != sequence.end())
                {
                    continue; // don't subdivide these two triangles just yet, wait till next iteration
                }
                if (triangle.dir == SierpinskiTriangle::SierpinskiDirection::AB_TO_BC && begin != sequence.begin())
                {
                    begin = std::prev(it);
                    assert((*begin)->depth == triangle.depth || isConstrainedBackward(it));
                }
                else
                {
                    assert(begin == std::prev(end));
                }
                bool is_constrained = isConstrainedBackward(begin) || isConstrainedForward(std::prev(end));
                // Don't check for constraining in between the cells in the range;
                // the range is defined as the range of triangles which are constraining each other simultaneously.

                if (node->depth == max_depth) // Never subdivide beyond maximum depth.
                    continue;
                float total_subdiv_error = getSubdivisionError(begin, end);
                if (!node->children.empty() && total_subdiv_error >= 0 && !is_constrained)
                {
                    bool redistribute_errors = true;
                    subdivide(begin, end, redistribute_errors);
                    change = true;
                }
            }
        return change;
    }

    bool SierpinskiFill::bubbleUpConstraintErrors()
    {
        std::vector<std::vector<std::list<SierpinskiTriangle*>::iterator>> depth_ordered = getDepthOrdered();

        bool redistributed_anything = false;

        for (int depth = max_depth; depth >= 0; depth--)
        {
            std::vector<std::list<SierpinskiTriangle*>::iterator>& depth_nodes = depth_ordered[depth];
            for (std::list<SierpinskiTriangle*>::iterator it : depth_nodes)
            {
                SierpinskiTriangle* node = *it;
                SierpinskiTriangle& triangle = *node;

                float unresolvable_error = triangle.getValueError();

                // If constrained in one direction, resolve the error in the other direction only.
                // If constrained in both directions, divide the error equally over both directions.
                bool is_constrained_forward = isConstrainedForward(it);
                bool is_constrained_backward = isConstrainedBackward(it);
                if (unresolvable_error > allowed_length_error && (is_constrained_forward || is_constrained_backward))
                {
                    if (is_constrained_forward && is_constrained_backward)
                    {
                        // when constrained in both directions, then divide error equally
                        unresolvable_error *= .5;
                        // disperse half of the error forward and the other half backward
                    }
                    if (deep_debug_checking)
                        debugCheck();
                    if (is_constrained_forward)
                    {
                        SierpinskiTriangle* next = *std::next(it);
                        node->error_right -= unresolvable_error;
                        next->error_left += unresolvable_error;
                    }
                    if (is_constrained_backward)
                    {
                        SierpinskiTriangle* prev = *std::prev(it);
                        node->error_left -= unresolvable_error;
                        prev->error_right += unresolvable_error;
                    }
                    if (std::abs(unresolvable_error) > allowed_length_error)
                    {
                        redistributed_anything = true;
                    }
                    if (deep_debug_checking)
                        debugCheck();
                }
            }
        }
        return redistributed_anything;
    }


    std::list<SierpinskiFill::SierpinskiTriangle*>::iterator SierpinskiFill::subdivide(std::list<SierpinskiTriangle*>::iterator begin, std::list<SierpinskiTriangle*>::iterator end, bool redistribute_errors)
    {
        if (redistribute_errors && deep_debug_checking)
            debugCheck();
        if (redistribute_errors)
        { // move left-over errors
            bool distribute_subdivision_errors = true;
            redistributeLeftoverErrors(begin, end, distribute_subdivision_errors);

            SierpinskiTriangle* first = *begin;
            SierpinskiTriangle* last = *std::prev(end);
            (*begin)->children.front().error_left += first->error_left;
            (*std::prev(end))->children.back().error_right += last->error_right;
        }
        if (redistribute_errors && deep_debug_checking)
            debugCheck(false);

        std::list<SierpinskiFill::SierpinskiTriangle*>::iterator first_child_it = std::prev(begin);
        // the actual subdivision
        for (std::list<SierpinskiTriangle*>::iterator it = begin; it != end; ++it)
        {
            SierpinskiTriangle* node = *it;
            assert(!node->children.empty() && "cannot subdivide node with no children!");
            for (SierpinskiTriangle& child : node->children)
            {
                sequence.insert(begin, &child);
            }
        }
        first_child_it++;
        // removal of parents
        std::list<SierpinskiTriangle*>::iterator last_child_it = std::prev(begin);
        sequence.erase(begin, end);

        if (redistribute_errors && deep_debug_checking)
            debugCheck(false);

        if (redistribute_errors)
        { // make positive errors in children well balanced
            // Pass along error from parent
            balanceErrors(first_child_it, std::next(last_child_it));
        }

        if (redistribute_errors && deep_debug_checking)
            debugCheck();

        return last_child_it;
    }

    void SierpinskiFill::redistributeLeftoverErrors(std::list<SierpinskiTriangle*>::iterator begin, std::list<SierpinskiTriangle*>::iterator end, bool distribute_subdivision_errors)
    {
        SierpinskiTriangle* prev = *std::prev(begin);
        SierpinskiTriangle* next = *end;
        SierpinskiTriangle* first = *begin;
        SierpinskiTriangle* last = *std::prev(end);

        // exchange intermediate errors
        for (auto it = begin; it != end && std::next(it) != end; ++it)
        {
            SierpinskiTriangle* node = *it;
            SierpinskiTriangle* next = *std::next(it);
            if (std::abs(node->error_right + next->error_left) > allowed_length_error)
            {
                LOGW("Nodes aren't balanced! er: {} next el: {}", node->error_right, next->error_left);
                assert(false);
            }
            float exchange = node->error_right;
            if (node->error_right < next->error_left)
            {
                exchange *= -1;
            }
            node->error_right -= exchange;
            next->error_left += exchange;
        }

        float total_superfluous_error = 0;
        for (auto it = begin; it != end; ++it)
        {
            SierpinskiTriangle* node = *it;
            total_superfluous_error += (distribute_subdivision_errors) ? node->getSubdivisionError() : node->getValueError();
        }
        if (total_superfluous_error < allowed_length_error)
        { // there is no significant left-over error
            if (distribute_subdivision_errors && total_superfluous_error < -allowed_length_error)
            {
                LOGW("RedistributeLeftoverErrors shouldn't be called if the node isn't to be subdivided. Total error: {}", total_superfluous_error);
                assert(false);
            }
            return;
        }
        if (begin != sequence.begin() && end != sequence.end() && first->error_left > allowed_length_error && last->error_right > allowed_length_error)
        {
            float total_error_input = first->error_left + last->error_right;
            total_superfluous_error = std::min(total_superfluous_error, total_error_input); // total superfluous error cannot be more than the influx of error
            float left_spillover = total_superfluous_error * first->error_left / total_error_input;
            float right_spillover = total_superfluous_error * last->error_right / total_error_input;
            (*begin)->error_left -= left_spillover;
            prev->error_right += left_spillover;
            (*std::prev(end))->error_right -= right_spillover;
            next->error_left += right_spillover;
        }
        else if (begin != sequence.begin() && first->error_left > allowed_length_error)
        {
            total_superfluous_error = std::min(total_superfluous_error, first->error_left); // total superfluous error cannot be more than the influx of error
            (*begin)->error_left -= total_superfluous_error;
            prev->error_right += total_superfluous_error;
            assert(first->error_left > -allowed_length_error);
        }
        else if (end != sequence.end() && last->error_right > allowed_length_error)
        {
            total_superfluous_error = std::min(total_superfluous_error, last->error_right); // total superfluous error cannot be more than the influx of error
            last->error_right -= total_superfluous_error;
            next->error_left += total_superfluous_error;
            assert(last->error_right > -allowed_length_error);
        }
    }

    void SierpinskiFill::balanceErrors(std::list<SierpinskiFill::SierpinskiTriangle*>::iterator begin, std::list<SierpinskiFill::SierpinskiTriangle*>::iterator end)
    {
        // copy sublist to array
        std::vector<SierpinskiFill::SierpinskiTriangle*> nodes;
        for (std::list<SierpinskiFill::SierpinskiTriangle*>::iterator it = begin; it != end; ++it)
        {
            nodes.emplace_back(*it);
        }

        std::vector<float> node_error_compensation(nodes.size());

        // sort children on value_error, i.e. sort on total_value
        std::vector<int> order;
        for (unsigned int node_idx = 0; node_idx < nodes.size(); node_idx++)
        {
            order.emplace_back(node_idx);
        }
        std::sort(order.begin(), order.end(), [&nodes](int a, int b) { return nodes[a]->getValueError() < nodes[b]->getValueError(); });

        // add error to children with too low value
        float added = 0;
        unsigned int node_order_idx;
        for (node_order_idx = 0; node_order_idx < nodes.size(); node_order_idx++)
        {
            int node_idx = order[node_order_idx];
            SierpinskiTriangle* node = nodes[node_idx];
            float value_error = node->getValueError();
            if (value_error < 0)
            {
                added -= value_error;
                node_error_compensation[node_idx] = -value_error;
            }
            else
                break;
        }
        if (added == 0)
        {
            return;
        }

        // subtract the added value from remaining children
        // divide acquired negative balancing error among remaining nodes with positive value error
        float subtracted = 0;
        // divide up added among remaining children in ratio to their value error
        float total_remaining_value_error = 0;
        for (unsigned int remaining_node_order_idx = node_order_idx; remaining_node_order_idx < nodes.size(); remaining_node_order_idx++)
        {
            int node_idx = order[remaining_node_order_idx];
            SierpinskiTriangle* node = nodes[node_idx];
            float val_err = node->getValueError();
            assert(val_err > -allowed_length_error);
            total_remaining_value_error += val_err;
        }

        if (total_remaining_value_error < added - allowed_length_error)
        {
            LOGW("total_remaining: {} should be > {}", total_remaining_value_error, added);
            assert(false);
        }

        if (std::abs(total_remaining_value_error) < .0001) // Error is insignificant.
        {
            return;
        }

        for (unsigned int remaining_node_order_idx = node_order_idx; remaining_node_order_idx < nodes.size(); remaining_node_order_idx++)
        {
            int node_idx = order[remaining_node_order_idx];
            SierpinskiTriangle* node = nodes[node_idx];
            float val_error = node->getValueError();
            assert(val_error > -allowed_length_error);

            float diff = added * val_error / total_remaining_value_error;
            subtracted += diff;
            node_error_compensation[node_idx] = -diff;
        }
        if (std::abs(subtracted - added) > allowed_length_error)
        {
            LOGW("Redistribution didn't distribute well!! added {} subtracted {}", added, subtracted);
            assert(false);
        }

        float energy = 0;
        for (unsigned int node_idx = 0; node_idx < nodes.size(); node_idx++)
        {
            nodes[node_idx]->error_left -= energy;
            energy += node_error_compensation[node_idx];
            nodes[node_idx]->error_right += energy;
        }
        assert(energy < allowed_length_error);
    }

    void SierpinskiFill::settleErrors()
    {
        std::vector<std::vector<std::list<SierpinskiTriangle*>::iterator>> depth_ordered = getDepthOrdered();

        for (std::vector<std::list<SierpinskiTriangle*>::iterator>& depth_nodes : depth_ordered)
        {
            for (std::list<SierpinskiTriangle*>::iterator it : depth_nodes)
            {
                redistributeLeftoverErrors(it, std::next(it), false);
            }
        }
    }

    void SierpinskiFill::diffuseError()
    {
        int pair_constrained_nodes = 0;
        int constrained_nodes = 0;
        int unconstrained_nodes = 0;
        int subdivided_nodes = 0;
        float error = 0;
        for (std::list<SierpinskiTriangle*>::iterator it = sequence.begin(); it != sequence.end(); ++it)
        {
            SierpinskiTriangle& triangle = *(*it);

            float boundary = (triangle.realized_length + triangle.total_child_realized_length) * .5f;

            float nodal_value = ((use_errors_in_dithering) ? triangle.getErroredValue() : triangle.requested_length);

            float boundary_error = nodal_value - boundary + error;

            std::list<SierpinskiTriangle*>::iterator begin = it;
            std::list<SierpinskiTriangle*>::iterator end = std::next(it);
            if (triangle.dir == SierpinskiTriangle::SierpinskiDirection::AC_TO_AB && end != sequence.end())
            {
                pair_constrained_nodes++;
                continue; // don't subdivide these two triangles just yet, wait till next iteration
            }
            if (triangle.dir == SierpinskiTriangle::SierpinskiDirection::AB_TO_BC && begin != sequence.begin())
            {
                begin = std::prev(it);
                assert((*begin)->depth == triangle.depth || isConstrainedBackward(it));
            }


            bool is_constrained = false;
            for (auto nested_it = begin; nested_it != end; ++nested_it)
            {
                if (isConstrainedBackward(nested_it) || isConstrainedForward(nested_it))
                {
                    is_constrained = true;
                    constrained_nodes++;
                    break;
                }
            }
            if (!is_constrained)
                unconstrained_nodes++;
            if (!is_constrained && boundary_error >= 0 && !triangle.children.empty())
            {
                subdivided_nodes++;
                it = subdivide(begin, end, false);
                if (dithering)
                    error += nodal_value - triangle.total_child_realized_length;
            }
            else
            {
                if (dithering)
                    error += nodal_value - triangle.realized_length;
            }
        }
        LOGD("pair_constrained_nodes: {}, constrained_nodes: {}, unconstrained_nodes: {}, subdivided_nodes: {}", pair_constrained_nodes, constrained_nodes, unconstrained_nodes, subdivided_nodes);
    }

    bool SierpinskiFill::isConstrainedBackward(std::list<SierpinskiTriangle*>::iterator it)
    {
        SierpinskiTriangle* node = *it;
        SierpinskiTriangle* prev = *std::prev(it);
        if (it != sequence.begin() && node->dir == SierpinskiTriangle::SierpinskiDirection::AB_TO_BC && prev->depth < node->depth)
            return true;
        return false;
    }
    bool SierpinskiFill::isConstrainedForward(std::list<SierpinskiTriangle*>::iterator it)
    {
        SierpinskiTriangle* node = *it;
        SierpinskiTriangle* next = *std::next(it);
        if (std::next(it) != sequence.end() && node->dir == SierpinskiTriangle::SierpinskiDirection::AC_TO_AB && next->depth < node->depth)
            return true;
        return false;
    }

    float SierpinskiFill::getSubdivisionError(std::list<SierpinskiTriangle*>::iterator begin, std::list<SierpinskiTriangle*>::iterator end)
    {
        float ret = 0;
        for (auto it = begin; it != end; ++it)
        {
            SierpinskiTriangle* node = *it;
            ret += node->getSubdivisionError();
        }
        return ret;
    }


    void SierpinskiFill::debugOutput(SVG& svg)
    {
        //svg.writePolygon(aabb.toPolygon(), SVG::Color::RED);

        //// draw triangles
        //for (SierpinskiTriangle* node : sequence)
        //{
        //    SierpinskiTriangle& triangle = *node;
        //    svg.writeLine(triangle.a, triangle.b, SVG::Color::GRAY);
        //    svg.writeLine(triangle.a, triangle.straight_corner, SVG::Color::GRAY);
        //    svg.writeLine(triangle.b, triangle.straight_corner, SVG::Color::GRAY);
        //}
    }


    SierpinskiFill::Edge SierpinskiFill::SierpinskiTriangle::getFromEdge()
    {
        Edge ret;
        switch (dir)
        {
        case SierpinskiDirection::AB_TO_BC:
            ret = Edge{ a, b };
            break;
        case SierpinskiDirection::AC_TO_AB:
            ret = Edge{ straight_corner, a };
            break;
        case SierpinskiDirection::AC_TO_BC:
            ret = Edge{ straight_corner, a };
            break;
        }
        if (!straight_corner_is_left)
        {
            std::swap(ret.l, ret.r);
        }
        return ret;
    }

    SierpinskiFill::Edge SierpinskiFill::SierpinskiTriangle::getToEdge()
    {
        Edge ret;
        switch (dir)
        {
        case SierpinskiDirection::AB_TO_BC:
            ret = Edge{ straight_corner, b };
            break;
        case SierpinskiDirection::AC_TO_AB:
            ret = Edge{ b, a };
            break;
        case SierpinskiDirection::AC_TO_BC:
            ret = Edge{ straight_corner, b };
            break;
        }
        if (!straight_corner_is_left)
        {
            std::swap(ret.l, ret.r);
        }
        return ret;
    }

    float SierpinskiFill::SierpinskiTriangle::getTotalError()
    {
        return error_left + error_right;
    }

    float SierpinskiFill::SierpinskiTriangle::getErroredValue()
    {
        return requested_length + getTotalError();
    }

    float SierpinskiFill::SierpinskiTriangle::getSubdivisionError()
    {
        return getErroredValue() - total_child_realized_length;
    }

    float SierpinskiFill::SierpinskiTriangle::getValueError()
    {
        return getErroredValue() - realized_length;
    }


    Polygon SierpinskiFill::generateCross() const
    {
        Polygon ret;

        for (SierpinskiTriangle* max_level_it : sequence)
        {
            SierpinskiTriangle& triangle = *max_level_it;
            Point edge_middle = triangle.a + triangle.b + triangle.straight_corner;
            switch (triangle.dir)
            {
            case SierpinskiTriangle::SierpinskiDirection::AB_TO_BC:
                edge_middle -= triangle.a;
                break;
            case SierpinskiTriangle::SierpinskiDirection::AC_TO_AB:
                edge_middle -= triangle.straight_corner;
                break;
            case SierpinskiTriangle::SierpinskiDirection::AC_TO_BC:
                edge_middle -= triangle.a;
                break;
            }
            ret.append(edge_middle / 2);
        }

        float realized_length = INT2MM(ret.length());
        float requested_length = root.requested_length;
        float error = (realized_length - requested_length) / requested_length;
        LOGD("realized_length: {}, requested_length: {}  :: {}% error", realized_length, requested_length, .01 * static_cast<int>(10000 * error));
        return ret;
    }

    Polygon SierpinskiFill::generateCross(coord_t z, coord_t min_dist_to_side, coord_t pocket_size) const
    {
        Polygon ret;

        std::function<Point(int, Edge)> get_edge_crossing_location = [z, min_dist_to_side](const coord_t period, const Edge e)
        {
            coord_t from_l = z % (period * 2);
            if (from_l > period)
            {
                from_l = period * 2 - from_l;
            }
            from_l = from_l * vSize(e.l - e.r) / period;
            from_l = std::max(min_dist_to_side, from_l);
            from_l = std::min(vSize(e.l - e.r) - min_dist_to_side, from_l);
            return e.l + normal(e.r - e.l, from_l);
        };

        SierpinskiTriangle* last_triangle = nullptr;
        for (SierpinskiTriangle* node : sequence)
        {
            SierpinskiTriangle& triangle = *node;

            /* The length of a side of the triangle is used as the period of
            repetition. That way the edges overhang by not more than 45 degrees.

            While there is a vertex is moving back and forth on the diagonal between
            A and B, this doesn't cause a steeper angle of parallel lines in the
            Cross pattern because the other side is sliding along the straight
            sides. The steeper overhang is then only in the corner, which is deemed
            acceptable since the corners are never too sharp. */
            const coord_t period = vSize(triangle.straight_corner - triangle.a);
            ret.append(get_edge_crossing_location(period, triangle.getFromEdge()));

            last_triangle = &triangle;
        }
        assert(last_triangle);
        const coord_t period = vSize(last_triangle->straight_corner - last_triangle->a);
        ret.append(get_edge_crossing_location(period, last_triangle->getToEdge()));

        if (pocket_size > 10)
        {
            // round off corners by half square root 2 of the pocket size so that the whole hole will be sqrt_pocket_size wide
            // \      /     \      /
            //  \    /  ==>  \____/
            //   \  /}\       ^^^^--pocket_size / 2
            //    \/} / pocket_size_side
            coord_t pocket_size_side = pocket_size * sqrt2 / 2;

            Polygon pocketed;
            pocketed.points.reserve(ret.size() * 3 / 2);

            Point p0 = ret.back();
            for (size_t poly_idx = 0; poly_idx < ret.size(); poly_idx++)
            {
                Point p1 = ret[poly_idx];
                Point p2 = ret[(poly_idx + 1) % ret.size()];
                Point v0 = p0 - p1;
                Point v1 = p2 - p1;

                coord_t prod = std::abs(dot(v0, v1));
                bool is_straight_corner = prod < sqrt(vSize(v0)* vSize(v1))* min_dist_to_side; // allow for rounding errors of up to min_dist_to_side
                if (is_straight_corner)
                {
                    coord_t pocket_rounding = std::min(std::min(pocket_size_side, vSize(v0) / 3), vSize(v1) / 3); // a third so that if a line segment is shortened on both sides the middle remains
                    pocketed.append(p1 + normal(v0, pocket_rounding));
                    pocketed.append(p1 + normal(v1, pocket_rounding));
                }
                else
                {
                    pocketed.append(p1);
                }
                p0 = p1;
            }

            return pocketed;
        }
        else
        {
            return ret;
        }
    }

    void SierpinskiFill::debugCheck(bool check_subdivision)
    {
        if (std::abs(sequence.front()->error_left) > allowed_length_error)
        {
            LOGW("First node has error left!");
            assert(false);
        }
        if (std::abs(sequence.back()->error_right) > allowed_length_error)
        {
            LOGW("Last node has error right!");
            assert(false);
        }

        for (auto it = sequence.begin(); it != sequence.end(); ++it)
        {
            if (std::next(it) == sequence.end())
            {
                break;
            }
            SierpinskiTriangle* node = *it;
            SierpinskiTriangle* next = *std::next(it);

            if (std::abs(node->error_right + next->error_left) > allowed_length_error)
            {
                LOGW("Consecutive nodes in fractal don't have the same error! er: {} , nel: {}", node->error_right, next->error_left);
                assert(false);
            }
            if (check_subdivision && node->getValueError() < -allowed_length_error)
            {
                LOGW("Fractal node shouldn't have been subdivided!");
                assert(false);
            }
        }
    }

} // namespace cura52
