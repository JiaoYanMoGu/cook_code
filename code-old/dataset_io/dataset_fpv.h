#ifndef DATASET_FPV_H
#define DATASET_FPV_H

#include "dataset_io.h"

namespace io {

///
/// \brief IROS2019 FPV Drone Race Dataset
/// http://rpg.ifi.uzh.ch/uzh-fpv.html
///

class DatasetFPV : public Dataset{
public:
    DatasetFPV(const std::string &sequence);
    virtual ~DatasetFPV() override{}
    virtual bool load() override;
    virtual size_t get_num_cams() const override;
};


} // namespace io


#endif // DATASET_FPV_H
