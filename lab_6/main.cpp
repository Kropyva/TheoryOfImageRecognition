#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

// Task 3
int main(int, char**) {
    auto path = "D:/University/Third/Second/TheoryOfRecognizeImages/MyLabs/lab_6/haarcascade_frontalface_default.xml";
    cv::VideoCapture capture(0);

    if (!capture.isOpened()) {
        std::cout << "Failed to open webcam!" << std::endl;
        return -1;
    }

    cv::CascadeClassifier cascade;
    if(!cascade.load(path)) {
        std::cout << "Failed to load cascade!" << std::endl;
    }

    while(true) {
        cv::Mat frame;
        capture.read(frame);

        if (frame.empty()) {
            std::cout << "Failed to capture frame!" << std::endl;
            break;
        }
        
        cv::Mat gray_frame;
        cv::cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY);

        std::vector<cv::Rect> faces;
        cascade.detectMultiScale(gray_frame, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

        for (const auto& face : faces) {
            cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);
        }

        cv::imshow("WebCam", frame);

        if (cv::waitKey(1) == 27) {
            break;
        }
    };

    capture.release();
    cv::destroyAllWindows();

    return 0;
}

/* // Task 2
int main(int, char**) {
    cv::VideoCapture capture(0);

    if (!capture.isOpened()) {
        std::cout << "Failed to open webcam!" << std::endl;
        return -1;
    }

    while(true) {
        cv::Mat frame;
        capture.read(frame);

        if (frame.empty()) {
            std::cout << "Failed to capture frame!" << std::endl;
            break;
        }

        cv::Mat edges;
        cv::cvtColor(frame, edges, cv::COLOR_BGR2GRAY);
        cv::Canny(edges, edges, 30, 60);

        if (edges.empty()) {
            std::cout << "Failed to capture edges!" << std::endl;
        }

        cv::imshow("WebCam", edges);

        if (cv::waitKey(1) == 27) {
            break;
        }
    };

    capture.release();
    cv::destroyAllWindows();

    return 0;
} */

/* // Taks 1
int main(int, char**) {
    cv::VideoCapture capture(0);

    if (!capture.isOpened()) {
        std::cout << "Failed to open webcam!" << std::endl;
        return -1;
    }

    while(true) {
        cv::Mat frame;
        capture.read(frame);

        if (frame.empty()) {
            std::cout << "Failed to capture frame!" << std::endl;
            break;
        }

        cv::imshow("WebCam", frame);

        if (cv::waitKey(1) == 27) {
            break;
        }
    };

    capture.release();
    cv::destroyAllWindows();

    return 0;
} */