#include <cam0.h>
#include <dlfcn.h>
#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>
#include <vector>

struct CvRelease {
    cv::VideoCapture& m_vc;

    CvRelease(cv::VideoCapture vc)
        : m_vc(vc)
    {
    }

    ~CvRelease()
    {
        m_vc.release();
        cv::destroyAllWindows();
    }
};

struct FilterLoadError : public std::exception {
    char const* what() const throw()
    {
        return dlerror();
    }
};

struct Filters {
    std::vector<void*> m_handles;
    std::vector<cam0::filter> m_filters;

    Filters() { }

    Filters(int argc, char const** argv)
    {
        for (int i = 1; i < argc; i++) {
            void* handle = dlopen(argv[i], RTLD_LAZY);
            if (handle == NULL)
                throw FilterLoadError();
            m_handles.push_back(handle);
            void* filter = dlsym(handle, "filter");
            if (filter == NULL)
                throw FilterLoadError();
            m_filters.push_back((cam0::filter)filter);
        }
    }

    ~Filters()
    {
        for (size_t i = 0; i < m_handles.size(); i++)
            (void)dlclose(m_handles[i]);
    }

    void apply(cv::Mat& im)
    {
        for (size_t i = 0; i < m_filters.size(); i++)
            m_filters[i](im);
    }
};

int main(int argc, char const* argv[])
{
    std::unique_ptr<Filters> filters;
    try {
        filters = std::move(std::make_unique<Filters>(argc, argv));
    } catch (FilterLoadError const& e) {
        std::cerr << "could not load filters: " << e.what() << std::endl;
        return 1;
    }
    cv::VideoCapture cam0(0);
    if (!cam0.isOpened()) {
        std::cerr << "could not open camera" << std::endl;
        return 1;
    }
    // CvRelease defer_cam0(cam0);
    while (true) {
        cv::Mat im;
        if (!cam0.read(im)) {
            std::cerr << "camera disconnected" << std::endl;
            return 1;
        }
        filters->apply(im);
        cv::imshow("cam0", im);
        if (cv::waitKey(1) == 27)
            return 0;
    }
}
