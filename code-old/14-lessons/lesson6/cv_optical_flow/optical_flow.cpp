#include "optical_flow.h"

void viz_opticalflow(const std::string& win_name,const Mat &img2, const vector<KeyPoint> &kp1, vector<KeyPoint> &kp2, vector<bool> &success_array)
{
    cv::Mat image2draw;
    cvtColor(img2,image2draw,CV_GRAY2BGR);
    for(size_t  i = 0; i < kp1.size() ; ++i)
    {
        if(success_array[i])
        {
            cv::circle(image2draw,kp2[i].pt,2,cv::Scalar(0,255,0));
            cv::line(image2draw,kp1[i].pt,kp2[i].pt,cv::Scalar(0,0,255));
        }
        else{
            cv::circle(image2draw,kp1[i].pt,2,cv::Scalar(255,255,255));
        }
    }
    cv::namedWindow(win_name,WINDOW_AUTOSIZE);
    imshow(win_name,image2draw);
    cv::waitKey(0);
}
void viz_opticalflow(const std::string& win_name,const Mat &img2, const vector<Point2f> &kp1, vector<Point2f> &kp2, vector<uchar> &success_array)
{
    cv::Mat image2draw;
    cvtColor(img2,image2draw,CV_GRAY2BGR);
    for(size_t  i = 0; i < kp1.size() ; ++i)
    {
        if(success_array[i])
        {
            cv::circle(image2draw,kp2[i],2,cv::Scalar(0,255,0));
            cv::line(image2draw,kp1[i],kp2[i],cv::Scalar(0,0,255));
        }
        else{
            cv::circle(image2draw,kp1[i],2,cv::Scalar(255,255,255));
        }
    }
    imshow(win_name,image2draw);
    cv::waitKey(0);
}

//TermCriteria criteria = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 0.01),
void OpticalFlowSingleLevel(
        const Mat &img1,
        const Mat &img2,
        const vector<KeyPoint> &kp1,
        vector<KeyPoint> &kp2,
        vector<bool> &success_array,
        bool inverse,
        bool has_initial,
        TermCriteria criteria)
{

    if(success_array.empty())
    {
        success_array.resize(kp1.size(),false);
    }
    if(kp2.empty())
    {
        kp2.resize(kp1.size());
    }
    int half_patch_size = 20;

    // compute gradient use sobel template
    cv::Mat gx1,gy1;
    cv::spatialGradient(img1,gx1,gy1);
    cv::Mat gx2,gy2;
    cv::spatialGradient(img2,gx2,gy2);

    for(size_t pt_index = 0; pt_index < kp1.size() ; ++pt_index)
    {
        float initial_dx = 0.0;
        float initial_dy = 0.0;
        auto kp = kp1[pt_index].pt;
        if(has_initial)
        {
            initial_dx = kp2[pt_index].pt.x - kp.x;
            initial_dx = kp2[pt_index].pt.y - kp.y;
        }

        float cost = 0.0, lastCost = 0.0;
        bool success = false;

        for(int i= 0;i < criteria.maxCount; i++)
        {
            cv::Matx22f H =cv::Matx22d::zeros();
            cv::Vec2f b(0.0f,0.0f);
            cost =0.0;

            if(kp.x + initial_dx < -half_patch_size || kp.x + initial_dx > img1.cols - half_patch_size ||
                   kp.y +initial_dy <- half_patch_size || kp.y + initial_dy > img1.rows - half_patch_size)
            {
                success = false;
                break;
            }
            for(int y = -half_patch_size; y < half_patch_size ;++y)
                for(int x = -half_patch_size; x < half_patch_size ;++x)
                {
                    float u1,u2,v1,v2;
                    u1 = kp.x + x;
                    u2 = u1 + initial_dx;
                    v1 = kp.y + y;
                    v2 = v1 + initial_dy;
                    float error = 0.0;
                    error = GetPixelValue(img2,u2,v2) - GetPixelValue(img1,u1,v1);
                    Vec2f J;
                    if(inverse == false)
                    {
                        J[0] = gx1.at<float>(kp.x+x,kp.y+y);// (GetPixelValue(img2,u2+1,v2)-GetPixelValue(img2,u2-1,v2))/2;//dx
                        J[1] = gy1.at<float>(kp.x+x,kp.y+y);//(GetPixelValue(img2,u2,v2+1)-GetPixelValue(img2,u2,v2-1))/2;//dy
                    }
                    else{
                        J[0] = (GetPixelValue(img1,u1+1,v1)-GetPixelValue(img1,u1-1,v1))/2;//dx
                        J[1] = (GetPixelValue(img1,u1,v1+1)-GetPixelValue(img1,u1,v1-1))/2;//dy
                    }
                    H += J*J.t();
                    b += -error*J;
                    cost += error*error;
                }
            Vec2f update;
            solve(H,b,update,DECOMP_LU);

            if(isnanf(update[0]))
            {
                cout<<"update is nan"<<endl;
                success = false;
                break;
            }
            if(cv::norm(update) < criteria.epsilon)
            {
                #ifdef DEBUG
                cout<<" update norm: "<<cv::norm(update)<<" less than EPS"<<endl;
                #endif
                break;
            }
            if(i>0 && cost > lastCost)
            {
                #ifdef DEBUG
                cout<<"cost increased: "<<cost<<", "<<lastCost<<endl;
                #endif
                break;
            }
            initial_dx += update[0];
            initial_dy += update[1];
            lastCost = cost;
            success = true;
        }
        success_array[pt_index] = success;
        if(has_initial){
            kp2[pt_index].pt = kp + Point2f(initial_dx,initial_dy);
        }else{
             KeyPoint tracked;
             tracked.pt = kp;
             tracked.pt += cv::Point2f(initial_dx,initial_dy);
             kp2[pt_index]=tracked;
        }
    }


}

void OpticalFlowMultiLevel(const Mat &img1, const Mat &img2, const vector<KeyPoint> &kp1,
                           vector<KeyPoint> &kp2, vector<bool> &success_array, bool inverse, TermCriteria criteria)
{
    // 构建金字塔
    const int level = 5;
    const double scale = 1.2;
    std::vector<double> scales;
    std::vector<double> inv_scales;
    for(int i=0 ; i < level; i++)
    {
        scales.push_back(pow(scale,i));
    }
    for(auto value :scales)
    {
        inv_scales.push_back(1.0/value);
    }

    // build image pyramid
    std::vector<Mat> image1_pyramid;
    std::vector<Mat> image2_pyramid;
    for(int i=0 ; i < level ; i++)
    {
        cv::Mat temp;
        Size sz(cvRound(float(img1.cols*inv_scales[i])), cvRound(float(img1.rows*inv_scales[i])));
        cv::resize(img1,temp,sz);
        image1_pyramid.push_back(temp);
        cv::Mat temp2;
        cv::resize(img2,temp2,sz);
        image2_pyramid.push_back(temp2);
    }


    // 从第一层开始track
    for(int i=level-1; i >0 ; --i )
    {
        // change to iteration
        std::vector<cv::KeyPoint> kp1_pyramid(kp1.begin(),kp1.end());
        for(auto& pt : kp1_pyramid)
        {
            pt.pt *= inv_scales[i];
        }
        // 缺少判断是否成功track了? 由粗到细会进行优化，还是必须重新跟踪
        OpticalFlowSingleLevel(image1_pyramid[i],image2_pyramid[i],kp1_pyramid,kp2,success_array,inverse,(i!=level-1));
        viz_opticalflow("DEBUG",image2_pyramid[i],kp1_pyramid,kp2,success_array);
        // save result
        for(auto& pt2 : kp2)
        {
            pt2.pt*=scale;
        }
    }
    OpticalFlowSingleLevel(image1_pyramid[0],image2_pyramid[0],kp1,kp2,success_array,inverse,true);
}



