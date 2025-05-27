#include "supportutils.h"

using namespace trimesh;
namespace creative_kernel
{
	bool checkSupportNormal(vec3& normal, float theta)
	{
		vec3 n(0.0f, 0.0f, -1.0f);
		normalize(normal);
		return dot(n, normal) >= (cosf(M_PIf * theta / 180.0f));
	}
}