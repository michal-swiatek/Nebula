//
// Created by michal-swiatek on 10.06.2022.
// Github: https://github.com/michal-swiatek/
//

#ifndef NEBULA_ENTRYPOINT_H
#define NEBULA_ENTRYPOINT_H

#if defined(_WIN32) || defined(WIN32)

int main(int argc, char** argv)
{
    nebula::Logger::Init();

    auto* app = nebula::CreateApplication();
    app->Run();

    delete app;

    return 0;
}

#endif

#endif //NEBULA_ENTRYPOINT_H
