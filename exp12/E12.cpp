//#include "opencv.hpp"
//#include "opencv2/nonfree/nonfree.hpp"
//#include <iostream>
//using namespace cv;
//using namespace std;
//
//int main()
//{
//    Mat img = imread("p3.png"), img_gray;
//    cvtColor(img, img_gray, COLOR_BGR2GRAY);
//    vector<KeyPoint> keypoints1, keypoints2, k1, k2;
//    Mat mask, des1, des2;
//    SURF surf1, surf2;
//    surf1(img_gray, mask, keypoints1, des1);
//
//    /*OrbFeatureDetector detector;
//    detector.detect(img, keypoints1);
//    OrbDescriptorExtractor extractor;
//    extractor.compute(img_gray, keypoints1, des1);*/
//
//    BFMatcher matcher;
//    vector<DMatch> matches;
//
//    int first = 1, num = 0;
//    Mat frame, frame_gray1;
//    VideoCapture capture("V3.mp4");
//    VideoWriter writer("VideoTest.avi", CV_FOURCC('X', 'V', 'I', 'D'), 25.0, Size(640, 480));
//
//    while (true) {
//        ++num;
//        double t = (double)getTickCount();
//
//        capture >> frame;
//        if (frame.empty() || waitKey(15) == char(27))
//            break;
//        if (first) {
//            first = 0;
//            cvtColor(frame, frame_gray1, COLOR_BGR2GRAY);
//            surf2(frame_gray1, mask, keypoints2, des2);
//            
//            //detector.detect(frame, keypoints2);
//            //extractor.compute(frame_gray1, keypoints2, des2);
//
//            matcher.match(des1, des2, matches);
//            sort(matches.begin(), matches.end());
//            matches.erase(matches.begin() + 60, matches.end());
//            vector<Point2f> kps1, kps2;
//            for (int i = 0; i < matches.size(); ++i) {
//                kps1.push_back(keypoints1[matches[i].queryIdx].pt);
//                kps2.push_back(keypoints2[matches[i].trainIdx].pt);
//                matches[i].queryIdx = i;
//                matches[i].trainIdx = i;
//            }
//            KeyPoint::convert(kps1, k1);
//            KeyPoint::convert(kps2, k2);
//
//            t = ((double)getTickCount() - t) / getTickFrequency();
//            double fps = 1.0 / t;
//            char string[10];
//            sprintf(string, "%.2f", fps);
//            std::string fpsString("FPS:");
//            fpsString += string;
//            putText(frame, fpsString, Point(5, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
//            cout << fps << endl;
//            Mat outputImg;
//            drawMatches(img, k1, frame, k2, matches, outputImg,
//                Scalar::all(-1), Scalar::all(-1), vector<char>(), 2);
//            imshow("output", outputImg);
//            //waitKey(0);
//            resize(outputImg, outputImg, Size(640, 480));
//            writer << outputImg;
//        }
//        else {
//            if (k2.size()<20) {
//                first = 1;
//                continue;
//            }
//            vector<Point2f> corners1, corners2;
//            KeyPoint::convert(k2, corners1);
//
//            Mat frame_gray2;
//            cvtColor(frame, frame_gray2, COLOR_BGR2GRAY);
//            Size winSize(5, 5);
//            TermCriteria criteria = TermCriteria(
//                TermCriteria::MAX_ITER + TermCriteria::EPS,
//                40,
//                0.03);
//            cornerSubPix(frame_gray1, corners1, winSize, Size(-1, -1), criteria);
//
//            vector<uchar> status;
//            vector<float> err;
//            calcOpticalFlowPyrLK(frame_gray1, frame_gray2, corners1, corners2, status, err, winSize, 5, criteria, 0, 0.001);
//
//            int cnt = 0;
//            for (int i = 0; i < corners2.size(); i++) {
//                if (status[i]) {
//                    if (corners1[i] != corners2[i])
//                        ++cnt;
//                    line(frame, corners1[i], corners2[i], Scalar(0, 255, 255));
//                    circle(frame, corners2[i], 3, Scalar(0, 0, 255), -1);
//                }
//            }
//            //if(cnt) cout << cnt << endl;
//
//            KeyPoint::convert(corners2, k2);
//            vector<Point2f> kps1, kps2;
//            int idx = 0;
//            for (int i = 0; i < matches.size(); ++i) {
//                if (status[i]) {
//                    kps1.push_back(k1[matches[i].queryIdx].pt);
//                    kps2.push_back(k2[matches[i].trainIdx].pt);
//                    matches[idx].queryIdx = idx;
//                    matches[idx].trainIdx = idx++;
//                }
//            }
//            matches.erase(matches.begin() + idx, matches.end());
//            KeyPoint::convert(kps1, k1);
//            KeyPoint::convert(kps2, k2);
//
//            t = ((double)getTickCount() - t) / getTickFrequency();
//            double fps = 1.0 / t;
//            char string[10];
//            sprintf(string, "%.2f", fps);
//            std::string fpsString("FPS:");
//            fpsString += string;
//            putText(frame, fpsString, Point(5, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
//
//            Mat outputImg;
//            drawMatches(img, k1, frame, k2, matches, outputImg,
//                Scalar::all(-1), Scalar::all(-1), vector<char>());
//            imshow("output", outputImg);
//            resize(outputImg, outputImg, Size(640, 480));
//            writer << outputImg;
//            frame_gray1 = frame_gray2;
//        }
//
//        //if (num % 5 == 1) {
//        //    cvtColor(frame, frame_gray1, COLOR_BGR2GRAY);
//        //    //first = 1;
//        //}
//        waitKey(40);
//    }
//    capture.release();
//    writer.release();
//    destroyAllWindows();
//    return 0;
//}