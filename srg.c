
#include "WavUtil.h"
#include "math.h"

#define SAMPLE_RATE 44100
#define BPS 32
#define WAVE_LENGTH 10000000
#define OUTPUT_FILE_NAME "shepard-risset.wav"

#define JMAX 5
#define NOTE_CENTER 72.0
#define PERIOD (SAMPLE_RATE * 2)

double ampForNote(double n) {
    return exp(-(n - NOTE_CENTER - 9) * (n - NOTE_CENTER - 9) / 24.0) / 3.0;
}

int main(int args, char **argv) {
    MonoWAV outputWav;
    char *exportFileName = OUTPUT_FILE_NAME;

    outputWav.sampleRate = SAMPLE_RATE;
    outputWav.bps = BPS;
    outputWav.length = WAVE_LENGTH;

    printf("Allocating memory..\n");
    outputWav.data = calloc(outputWav.length, sizeof(double));

    printf("Generating wave data...\n");
    double note = NOTE_CENTER;
    unsigned long term = 0;
    for (unsigned long i = 0; i < WAVE_LENGTH; i++) {
        double wave = 0.0;

        // if (i % 1000000 == 0) {
        //     printf("note = %lf\n", note);
        // }
        for (int j = -JMAX; j <= JMAX; j++) {
            double currentNote = note - 6.0 * j;
            double freq = pow(2.0, currentNote / 12.0) * 6.875;
            double amp = ampForNote(currentNote);
            double degree = (double)term / PERIOD;
            wave += amp * sin(degree * 2 * M_PI * freq);
            // if (i % 1000000 == 0) {
            //     printf("currentNote : %lf, ampForNote : %lf\n", currentNote, ampForNote(currentNote));
            // }
        }

        outputWav.data[i] = wave;

        term++;
        note = NOTE_CENTER - ((double)term / PERIOD);
        if (note <= NOTE_CENTER - 5.515) {
            term = 0;
            note = NOTE_CENTER - ((double)term / PERIOD);
        }
    }

    printf("Exporting wav file...\n");
    exportMonoWav(&outputWav, exportFileName);

    printf("Complete!\n");

    free(outputWav.data);
    
    return 0;
}
