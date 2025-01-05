/*
 * SPDX-License-Identifier: LGPL-2.1-or-later
 * SPDX-FileCopyrightText: Copyright 2021-2023 Fcitx5 for Android Contributors

 * SPDX-License-Identifier: GPL-3.0-only
 * SPDX-FileCopyrightText: Copyright 2025 Qijia Liu
 */
#pragma once

#include <array>
#include <iostream>
#include <streambuf>
#include "hilog/log.h"
#undef LOG_TAG
#define LOG_TAG "fcitx"

// hilog limit
template <std::size_t SIZE = 4096> class native_streambuf : public std::streambuf {
public:
    using Base = std::streambuf;
    using char_type = typename Base::char_type;
    using int_type = typename Base::int_type;

    native_streambuf() : _buffer{} { Base::setp(_buffer.begin(), _buffer.end() - 1); }

    // buffer is full but current "line" of log hasn't finished
    int_type overflow(int_type ch) override {
        // append terminate character to the buffer (usually _buffer.end() when
        // overflow)
        *Base::pptr() = '\0';
        const char *text = Base::pbase();
        if (should_offset) {
            // it's the first write of this "line", guess priority
            update_log_priority(text[0]);
            // this write would skip first character
            write_log(text);
            // consequence write of this "line" should use same priority and
            // should not skip characters
            should_offset = false;
        } else {
            // it's not the first write of this "line", so just write
            write_log(text);
        }
        // mark buffer as available, since it's content has been written to
        // android log but we need to preserve the last position for '\0' in
        // case it overflows
        Base::setp(_buffer.begin(), _buffer.end() - 1);
        // write 'ch' as char if it's not eof
        if (!Base::traits_type::eq_int_type(ch, Base::traits_type::eof())) {
            const char_type c = Base::traits_type::to_char_type(ch);
            Base::xsputn(&c, 1);
        }
        return 0;
    }

    // current "line" of log has finished, and buffer is not full
    int_type sync() override {
        *Base::pptr() = '\0';
        const char *text = Base::pbase();
        if (should_offset) {
            // it's the first write of this "line", guess priority
            update_log_priority(text[0]);
        }
        write_log(text);
        // this "line" has finished and written to NS log,
        // reset state for next "line"
        should_offset = true;
        // mark buffer as available and preserve last position for '\0'
        Base::setp(_buffer.begin(), _buffer.end() - 1);
        return 0;
    }

private:
    std::array<char_type, SIZE> _buffer;
    LogLevel prio;
    /**
     * whether the first character in buffer represents log level or not
     */
    bool should_offset = true;

    void update_log_priority(const char_type first) {
        switch (first) {
        case 'D':
            prio = LOG_DEBUG;
            break;
        case 'I':
            prio = LOG_INFO;
            break;
        case 'W':
            prio = LOG_WARN;
            break;
        case 'E':
            prio = LOG_ERROR;
            break;
        case 'F':
            prio = LOG_FATAL;
            break;
        default:
            break;
        }
    }

    void write_log(const char_type *text) const {
        OH_LOG_Print(LOG_APP, prio, LOG_DOMAIN, LOG_TAG, "%{public}s", text + (should_offset ? 1 : 0));
    }
};
