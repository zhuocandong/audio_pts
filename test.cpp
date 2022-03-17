/*******************************************************************************
 *  FILENAME:      test.cpp
 *
 *  AUTHORS:       zhuocandong    START DATE: Wednesday March 16th 2022
 *
 *  LAST MODIFIED: Thursday, March 17th 2022, 5:45:07 pm
 *
 *  CONTACT:       candong.zhuo@smartmore.com
 *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#define FAB(a, b) ((a > b) ? (a - b) : (b - a))

struct fifo
{
    int size = 0;
};

int audio_fifo_size(struct fifo &_fifo)
{
    return _fifo.size;
}

int ReadFrameFromFifo(struct fifo &_fifo, int out_nb)
{
    _fifo.size -= out_nb;
    return 0;
}

int main(int argc, char **argv)
{
    struct fifo foo;
    int in_nb = atoi(argv[1]);
    int out_nb = atoi(argv[2]);
    int counts = 0;
    bool _b_got_first_frame = false;

    int in_t = in_nb;
    int out_t = out_nb;
    while (in_t != out_t)
    {
        if (in_t > out_t)
        {
            in_t = in_t - out_t;
        }
        else
        {
            out_t = out_t - in_t;
        }
    }
    int _max_conventions = in_t; // the maximum number of conventions
    int _in_factor = in_nb / _max_conventions;
    int _out_factor = out_nb / _max_conventions;
    int plus_num_ = 0;
    int minus_num_ = 0;

    while (counts < atoi(argv[3]))
    {
        int in_pts = counts++ * in_nb;
        foo.size += in_nb;

        do // FlushFifo( )
        {
            int in_num = in_pts / in_nb;

            if (!_b_got_first_frame)
            {
                if ((in_num % _out_factor) != 0)
                {
                    continue;
                }
                _b_got_first_frame = true;
            }
            else if (_b_got_first_frame && (in_num % _out_factor) == 0)
            {
                // reset plus_num and minuns_num
                plus_num_ = 0;
                minus_num_ = 0;
            }

            if (audio_fifo_size(foo) < out_nb)
            {
                minus_num_ += 1;
            }

            while (audio_fifo_size(foo) >= out_nb)
            {

                ReadFrameFromFifo(foo, out_nb);

                int out_num;
                if (in_nb > out_nb)
                    out_num = in_num + (in_num / _out_factor) * FAB(_in_factor, _out_factor) + plus_num_;
                else if (in_nb < out_nb)
                    out_num = in_num - (in_num / _out_factor) * FAB(_in_factor, _out_factor) - minus_num_;
                else
                    out_num = in_num;

                int out_pts = out_num * out_nb;

                if (audio_fifo_size(foo) >= out_nb)
                {
                    plus_num_ += 1;
                }

                printf("[%d]end fifo_size:%d, \t\t[%d]OutputPTS:%d\n", in_pts / in_nb, audio_fifo_size(foo), out_pts / out_nb, out_pts);
                // encode(_ofmt_ctx, _output_codec_context, tmp_frame, _output_stream_index);
            }
        } while (0);
    }
}