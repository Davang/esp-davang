#include <format>

#include "davang_uart_stream.hpp"


extern "C" void app_main()
{
    dvng::c_uart_stream stream;
    std::string std_string      = "this a std::string";
    std::string std_string_view = "this a std::string_view";

    bool true_var  = true;
    bool false_var = false;

    const char chararacters[] = "character sequence";
    char       chararacter    = '@';

    uint8_t  u8   = 1;
    uint16_t u16  = 300;
    uint32_t u32  = 90'410;
    uint64_t u64  = 70'000'000'000;
    size_t   size = 5;

    ssize_t ssize = -5;
    int8_t  i8    = -1;
    int16_t i16   = -120;
    int32_t i32   = -50'000;
    int64_t i64   = -50'000'000'000;

    float  f = 1.0 / 3.0;
    double d = 1.0 / 6.0;

    stream << "uart stream supported types" << dvng::uart_stream::END_LINE::LF;
    stream << "address : " << &stream << dvng::uart_stream::END_LINE::LF;

    stream << "std_string : " << std_string << dvng::uart_stream::END_LINE::LF;
    stream << "std_string : " << std_string_view << dvng::uart_stream::END_LINE::LF;

    stream << "_t : " << true_var << dvng::uart_stream::END_LINE::LF;
    stream << "_f : " << false_var << dvng::uart_stream::END_LINE::LF;

    stream << dvng::uart_stream::s_bool{ "this is true", "this is false" }
           << "_t : " << true_var << dvng::uart_stream::END_LINE::LF
           << "_f : " << false_var << dvng::uart_stream::END_LINE::LF;

    stream << "chararacters : " << chararacters << dvng::uart_stream::END_LINE::LF;
    stream << "chararacter : " << chararacter << dvng::uart_stream::END_LINE::LF;
    stream << "dvng::uart_stream::NUM_BASE::HEX" << dvng::uart_stream::NUM_BASE::HEX << dvng::uart_stream::END_LINE::LF;
    stream << "size : " << size << dvng::uart_stream::END_LINE::LF;
    stream << "u8 : " << u8 << dvng::uart_stream::END_LINE::LF;
    stream << "u16 : " << u16 << dvng::uart_stream::END_LINE::LF;
    stream << "u32 : " << u32 << dvng::uart_stream::END_LINE::LF;
    stream << "u64 : " << u64 << dvng::uart_stream::END_LINE::LF;

    stream << "dvng::uart_stream::NUM_BASE::DEC" << dvng::uart_stream::NUM_BASE::DEC << dvng::uart_stream::END_LINE::LF;
    stream << "ssize : " << ssize << dvng::uart_stream::END_LINE::LF;
    stream << "i8 : " << i8 << dvng::uart_stream::END_LINE::LF;
    stream << "i16 : " << i16 << dvng::uart_stream::END_LINE::LF;
    stream << "i32 : " << i32 << dvng::uart_stream::END_LINE::LF;
    stream << "i64 : " << i64 << dvng::uart_stream::END_LINE::LF;
    
    stream << dvng::uart_stream::s_dec_precision{ 8 } << "f : " << f << dvng::uart_stream::END_LINE::LF;
    stream << dvng::uart_stream::s_dec_precision{ 2 } << "d : " << d << dvng::uart_stream::END_LINE::LF;

    stream << std::format( "{} {} {} {}", __PRETTY_FUNCTION__, __LINE__, __DATE__, __TIME__ ) << dvng::uart_stream::END_LINE::LF;
}