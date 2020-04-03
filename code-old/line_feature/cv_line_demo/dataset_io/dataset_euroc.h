#ifndef DATASET_EUROC_H
#define DATASET_EUROC_H

#include "dataset_io.h"


namespace io {


///
/// \brief The DatasetEuRoc class
/// https://projects.asl.ethz.ch/datasets/doku.php?id=kmavvisualinertialdatasets.
///
class DatasetEuRoc : public Dataset{
public:
    DatasetEuRoc(const std::string &sequence);
    virtual ~DatasetEuRoc() override {}
    virtual bool load() override;
    virtual size_t get_num_cams() const override;
};

} // namespace io

#endif // DATASET_EUROC_H
