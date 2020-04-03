#ifndef DATASET_ZJU_H
#define DATASET_ZJU_H

#include "dataset_io.h"

namespace io {

///
/// \brief ZJU VIO Dataset
///
class DatasetZJU : public Dataset{
public:
    DatasetZJU(const std::string &sequence);
    virtual ~DatasetZJU() override{}
    virtual bool load() override;
    virtual size_t get_num_cams() const override;
};


} // namespace io

#endif // DATASET_ZJU_H
