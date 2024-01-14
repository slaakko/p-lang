// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.init.done;

import util.ansi;
import util.socket;

namespace util {

void Init()
{
    util::SocketInit();
    util::AnsiInit();
}

void Done()
{
    util::AnsiDone();
    util::SocketDone();
}

} // namespace util
