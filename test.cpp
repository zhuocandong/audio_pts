/*******************************************************************************
 *  FILENAME:      test.cpp
 *
 *  AUTHORS:       zhuocandong    START DATE: Wednesday March 16th 2022
 *
 *  LAST MODIFIED: Thursday, March 17th 2022, 10:29:16 am
 *
 *  CONTACT:       candong.zhuo@smartmore.com
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

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

    while (counts < 30)
    {
        int in_pts = counts++ * in_nb;
        int plus_num = 0;
        foo.size += in_nb;
        while (audio_fifo_size(foo) >= out_nb)
        {

            ReadFrameFromFifo(foo, out_nb);
            int out_pts = 0;

            int factor = out_nb / _max_conventions;

            int in_num = in_pts / in_nb;

            if (!_b_got_first_frame)
            {
                if ((in_num % factor) != 0)
                {
                    break;
                }
                _b_got_first_frame = true;
            }

            in_num += (in_num / factor) + plus_num;

            out_pts = in_num * out_nb;

            printf("[%d]end fifo_size:%d, \t\t[%d]OutputPTS:%d\n", in_pts / in_nb, audio_fifo_size(foo), out_pts / out_nb, out_pts);
            // encode(_ofmt_ctx, _output_codec_context, tmp_frame, _output_stream_index);
            // av_frame_free(&tmp_frame);
            if (audio_fifo_size(foo) == out_nb)
                plus_num = (in_nb - out_nb) / _max_conventions;
        }
    }
}