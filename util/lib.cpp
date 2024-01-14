// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.lib;

namespace util {

std::string PLangVersionStr()
{
    return "0.1.0";
}

std::string PLangRoot()
{
    std::string plangRoot;
#pragma warning(suppress : 4996)
    const char* plangRootEnv = std::getenv("PLANG_ROOT");
    if (plangRootEnv)
    {
        plangRoot = plangRootEnv;
    }
    if (plangRoot.empty())
    {
        throw std::runtime_error("please set 'PLANG_ROOT' environment variable to contain /path/to/plang directory.");
    }
    return plangRoot;
}

} // namespace util
