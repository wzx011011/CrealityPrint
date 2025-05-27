#ifndef SUBCOPY_WRITE_H
#define SUBCOPY_WRITE_H
#include "common_3mf.h"
#include "miniz.h"
#include "expat.h"

namespace common_3mf
{
    class SubCopy3MFImpl
    {
    public:
        SubCopy3MFImpl(const std::string& file_name);
        ~SubCopy3MFImpl();

        void write_2_file(const std::string& file_name);
    protected:
		bool m_open_success;
		mz_zip_archive m_in_archive;
    };
}

#endif // SUBCOPY_WRITE_H