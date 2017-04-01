#include "network.h"

#include <cstdlib>
#include <ncurses.h>

const char *hostname = "localhost";
int port = 12345;
int rows, cols;

WINDOW *winStatus;
WINDOW *winWeather;
WINDOW *winEvents;

void updateClkPll(double freq, double offset);
void updateLat(double lat, double acc_m);
void updateLon(double lon, double acc_m);
void updateAlt(double alt, double acc_m);

void updateConnection(int l, const char *msg);

int main(int argc, char **argv) {
    if(argc > 1) hostname = argv[1];
    if(argc > 2) port = atoi(argv[2]);

    initscr();
    getmaxyx(stdscr, rows, cols);
    raw();
    keypad(stdscr, true);
    noecho();
    refresh();

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_GREEN);
    init_pair(6, COLOR_WHITE, COLOR_YELLOW);
    init_pair(7, COLOR_WHITE, COLOR_RED);
    init_pair(8, COLOR_BLACK, COLOR_WHITE);

    attron(COLOR_PAIR(1));

    // Create Location Widget
    winStatus = newwin(20, 50, 0, 0);
    box(winStatus, 0 , 0);

    mvwhline(winStatus, 5, 1, 0, 48);
    wattron(winStatus, A_BOLD);
    mvwprintw(winStatus, 6, 2, "Station");
    mvwprintw(winStatus, 6, 12, "Time-Of-Flight");
    mvwprintw(winStatus, 6, 32, "Jitter");
    mvwprintw(winStatus, 6, 42, "SNR");
    wattroff(winStatus, A_BOLD);

    // Create Weather Widget
    winWeather = newwin(20, 30, 0, 50);
    box(winWeather, 0 , 0);
    mvwhline(winWeather, 2, 1, 0, 28);
    wattron(winWeather, A_BOLD);
    mvwprintw(winWeather, 1, 11, "Weather");
    wattroff(winWeather, A_BOLD);

    // Create Events Widget
    winEvents = newwin(rows - 21, 80, 20, 0);

    wrefresh(winStatus);
    wrefresh(winWeather);
    wrefresh(winEvents);

    updateClkPll(0, 0);
    updateLat(0, 0);
    updateLon(0, 0);
    updateAlt(0, 0);
    wrefresh(winStatus);

    updateConnection(0, "OK");

    for(;;) {
        int c = getch();
        if(c == 'q') {
            break;
        }
    }



    endwin();
    return 0;
}

void updateClkPll(double freq_ppb, double offset_ns) {
    wattron(winStatus, A_BOLD);
    mvwprintw(winStatus, 1, 2, "CLK FRQ: % 11.1lf ppb | CLK OFF: % 7.1lf ns", freq_ppb, offset_ns);
    wattroff(winStatus, A_BOLD);
}

void updateLat(double lat, double acc_m) {
    bool south = lat < 0;
    if(south) lat = -lat;

    int deg = (int)lat; lat = (lat - deg) * 60;
    int min = (int)lat; lat = (lat - deg) * 60;

    wattron(winStatus, A_BOLD);
    mvwprintw(winStatus, 2, 2, "Lat: %c% 4d % 3d\"% 7.3lf'     % 6.2lf m", south?'S':'N', deg, min, lat, acc_m);
    mvwaddch(winStatus, 2, 12, ACS_DEGREE);
    mvwaddch(winStatus, 2, 27, ACS_PLMINUS);
    wattroff(winStatus, A_BOLD);
}

void updateLon(double lon, double acc_m) {
    bool west = lon < 0;
    if(west) lon = -lon;

    int deg = (int)lon; lon = (lon - deg) * 60;
    int min = (int)lon; lon = (lon - deg) * 60;

    wattron(winStatus, A_BOLD);
    mvwprintw(winStatus, 3, 2, "Lon: %c% 4d % 3d\"% 7.3lf'     % 6.2lf m", west?'W':'E', deg, min, lon, acc_m);
    mvwaddch(winStatus, 3, 12, ACS_DEGREE);
    mvwaddch(winStatus, 3, 27, ACS_PLMINUS);
    wattroff(winStatus, A_BOLD);
}

void updateAlt(double alt, double acc_m) {
    wattron(winStatus, A_BOLD);
    mvwprintw(winStatus, 4, 2, "Alt:% 17.2lf m     % 6.2lf m", alt, acc_m);
    mvwaddch(winStatus, 4, 27, ACS_PLMINUS);
    wattroff(winStatus, A_BOLD);
}

void updateConnection(int l, const char *msg) {
    char host[32];
    sprintf(host, "%s:%d", hostname, port);

    attron(A_BOLD);
    attron(COLOR_PAIR(5+l));
    mvprintw(rows - 1, 0, "  Backend:% 31s% 36s  ", host, msg);
    attroff(A_BOLD);
    attroff(COLOR_PAIR(5+l));
}
