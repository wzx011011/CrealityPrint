#include "subcopy.h"
#include "miniz_extension.h"

namespace common_3mf
{
    SubCopy3MF::SubCopy3MF(const std::string& file_name)
        :impl(new SubCopy3MFImpl(file_name))
    {
    }

    SubCopy3MF::~SubCopy3MF()
    {
        delete impl;
    }

    void SubCopy3MF::write_2_file(const std::string& file_name)
    {
        impl->write_2_file(file_name);
    }

    SubCopy3MFImpl::SubCopy3MFImpl(const std::string& file_name)
    {
        m_open_success = false;
        mz_zip_zero_struct(&m_in_archive);
		if (open_zip_reader(&m_in_archive, file_name))
			m_open_success = true;
    }

    SubCopy3MFImpl::~SubCopy3MFImpl()
    {
        if(m_open_success)
			close_zip_reader(&m_in_archive);
    }

    void SubCopy3MFImpl::write_2_file(const std::string& file_name)
    {
        if (!m_open_success)
            return;

        mz_zip_archive out_archive;
        mz_zip_zero_struct(&out_archive);
        if (!open_zip_writer(&out_archive, file_name))
            return;

        for (mz_uint i = 0; i < mz_zip_reader_get_num_files(&m_in_archive); i++)
        {
            mz_zip_archive_file_stat file_stat;
            if (!mz_zip_reader_file_stat(&m_in_archive, i, &file_stat))
                continue;

            mz_zip_writer_add_from_zip_reader(&out_archive, &m_in_archive, i);
        }
        mz_zip_writer_finalize_archive(&out_archive);
        
        close_zip_writer(&out_archive);
    }
}