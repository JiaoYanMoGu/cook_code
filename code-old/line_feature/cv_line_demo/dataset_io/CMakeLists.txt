add_library(dataset_io dataset_io.h dataset_io.cpp
    timestamp.h timestamp.cpp split.h split.cpp
    dataset_euroc.h dataset_euroc.cpp
    dataset_fpv.h dataset_fpv.cpp
    dataset_zju.h dataset_zju.cpp)
target_link_libraries(dataset_io ${OpenCV_LIBS} )
