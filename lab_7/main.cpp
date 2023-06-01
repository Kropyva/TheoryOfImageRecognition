#include <cstdlib>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>


// Зчитати картинку з директорії, якщо ні, то завершити програму.
void read_image(cv::Mat& image, std::string& path)
{
    image = cv::imread(path);

    if (image.empty()) {
        std::cout << "Failed to open an image " << path << std::endl;
        exit(1);
    }
}

int main(int, char**)
{
    std::string path { "../../jpg/" };

    // Вивести початкову абетку.
    cv::Mat initial_alphabet {};
    read_image(initial_alphabet, path + "alphabet.jpg");
    cv::imshow("Alphabet", initial_alphabet);

    while (true) {
        // Зчитати літеру з клавіатури
        char request_letter { static_cast<char>(cv::waitKey(0)) };
        if (request_letter == 27) {
            break;
        }

        // Дозволяються тільки великі літери
        if (request_letter < 65 || request_letter > 90) {
            std::cout << "The wrong key pressed!" << std::endl;
            continue;
        }

        cv::Mat alphabet {};
        read_image(alphabet, path + "alphabet.jpg");

        cv::Mat temp_alphabet {};
        read_image(temp_alphabet, path + "alphabet.jpg");

        cv::Mat letter {};
        read_image(letter, path + request_letter + ".jpg");

        cv::Mat temp_letter {};
        read_image(temp_letter, path + request_letter + ".jpg");

        // Перевести зображення в COLOR_BGR2GRAY колір.
        cv::cvtColor(temp_alphabet, temp_alphabet, cv::COLOR_BGR2GRAY);
        cv::cvtColor(temp_letter, temp_letter, cv::COLOR_BGR2GRAY);

        // Розмити абетку за Гаусом та прибрати шуми.
        cv::GaussianBlur(temp_alphabet, temp_alphabet, cv::Size(5, 5), 0);
        cv::threshold(temp_alphabet, temp_alphabet, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

        // Обвести зображення контурами на чорному тлі.
        cv::Canny(temp_alphabet, temp_alphabet, 50, 150);
        cv::Canny(temp_letter, temp_letter, 50, 150);

        // Знайти схожість між пошуковою літерою та літерою з абетки.
        cv::Mat result {};
        cv::matchTemplate(temp_letter, temp_alphabet, result, cv::TM_CCOEFF_NORMED);

        // Знайти позицію літери в абетці.
        double min_value {}, max_value {};
        cv::Point min_loc {}, max_loc {};
        cv::minMaxLoc(result, &min_value, &max_value, &min_loc, &max_loc);

        int width { letter.cols };
        int height { letter.rows };
        cv::Point top_left(max_loc.x, max_loc.y);
        cv::Point bottom_right(max_loc.x + width, max_loc.y + height);
        cv::Scalar color(0, 255, 0);

        cv::rectangle(alphabet, top_left, bottom_right, color, 2);

        // Вивести абетки на екран, показати результат.
        cv::namedWindow("Letter", cv::WINDOW_NORMAL);
        cv::resizeWindow("Letter", width * 3, height * 3);
        cv::namedWindow("Temp Letter", cv::WINDOW_NORMAL);
        cv::resizeWindow("Temp Letter", width * 3, height * 3);
        
        cv::imshow("Letter", letter);
        cv::imshow("Alphabet", alphabet);
        cv::imshow("Temp Alphabet", temp_alphabet);
        cv::imshow("Temp Letter", temp_letter);
    }

    cv::destroyAllWindows();
    return 0;
}
