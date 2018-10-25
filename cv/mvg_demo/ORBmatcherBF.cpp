#include "ORBmatcherBF.h"
#include <iostream>

bool ORBmatcherBF::match_lowe_ratio(const cv::Mat &descriptors1, const cv::Mat &descriptors2, double ratio,
                                    std::vector<cv::DMatch> &result) const
{
    //Use Lowe Ratio to filter some matches
    //Good Match: dis(nearest_point) / dis(second_nearest) < ratio
    //ratio=0.6 strict
    //ratio=0.6~0.8 normal
    //ratio<0.4 no matches

    cv::BFMatcher matcher(cv::NORM_HAMMING2);
    std::vector<std::vector<cv::DMatch> > all_matches;

    //find the nearest two points
    matcher.knnMatch(descriptors1,descriptors2,all_matches,2);
    //filtered matches
    for (int i = 0; i < all_matches.size(); ++i) {
        if(all_matches[i][0].distance < all_matches[i][1].distance * ratio ) {
            result.push_back(all_matches[i][0]);
        }
    }

    if (result.size() < 10)
    {
        std::cout<<"Matches too less !"<<std::endl;
        return false;
    } else{
        return true;
    }
}



