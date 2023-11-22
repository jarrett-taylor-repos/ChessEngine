#include "../AlphaBeta/AlphaBeta.h"
using namespace AlphaBeta;

namespace PlayGame {
    string simgamefile = "../../UCI/simgames.txt";

    void playgame_setdepth(int depth, string fen, bool logging) {
        srand(5);
        Bitboard b(fen);
        ofstream log;
        ofstream simgames;
        simgames.open(simgamefile);

        ZTable* ztable = new ZTable;
        
        int i = 0;
        bool isGameOngoing = true;
        while (isGameOngoing  && i < 250) {
            int numnodes = 0;
            long start = clock();
            log.open("logs\\log_"+to_string(i)+".md");
            isGameOngoing = makeMoveSetDepth(b, log, logging, simgames, *ztable, numnodes, depth);
            log.close();
            long end = clock();
            i++;
            cout<<i<<"-searched "<<numnodes<<" nodes in "<<end-start<<"ms"<<endl;
        }
        cout<<endl;
    }

    void playgame_settime(int time, string fen, bool logging) {
        srand(5);
        Bitboard b(fen);
        ofstream log;
        ofstream simgames;
        simgames.open(simgamefile);

        ZTable* ztable = new ZTable;

        int i = 0;
        bool isGameOngoing = true;
        while (isGameOngoing  && i < 250) {
            int numnodes = 0;
            long start = clock();
            int maxdepth = 0;
            log.open("logs\\log_"+to_string(i)+".md");
            isGameOngoing = makeMoveSetTime(b, log, logging, simgames, *ztable, numnodes, maxdepth, time);
            log.close();
            long end = clock();
            i++;
            cout<<i<<"-searched "<<numnodes<<" nodes in "<<end-start<<"ms up to depth "<<maxdepth<<endl;
        }
        cout<<endl;
    }

    void playgame_against_setdepth(int depth, string fen, bool logging) {
        srand(5);
        Bitboard b(fen);
        ofstream log;
        ofstream simgames;
        simgames.open(simgamefile);

        ZTable* ztable = new ZTable;
        
        int i = 0;
        bool isGameOngoing = true;
        while (isGameOngoing  && i < 250) {
            int numnodes = 0;
            long start = clock();
            int maxdepth = 0;
            log.open("logs\\log_"+to_string(i)+".md");
            isGameOngoing = makeMoveSetDepth(b, log, logging, simgames, *ztable, numnodes, depth);
            log.close();
            i++;
            cout<<b.GetFen()<<endl;
            string humanMove;
            bool moveMade = false;
            while (!moveMade) {
                cin>>humanMove;
                moveMade = b.MakeMoveFromUci(humanMove);
                if (!moveMade) {
                    cout<<"bad move"<<endl;
                }
            }
            simgames<<humanMove<<endl;
        }
        cout<<endl;
    }

    void playgame_against_settime(int time, string fen, bool logging) {
        srand(5);
        Bitboard b(fen);
        ofstream log;
        ofstream simgames;
        simgames.open(simgamefile);

        ZTable* ztable = new ZTable;
        
        int i = 0;
        bool isGameOngoing = true;
        while (isGameOngoing  && i < 250) {
            int numnodes = 0;
            long start = clock();
            int maxdepth = 0;
            log.open("logs\\log_"+to_string(i)+".md");
            isGameOngoing = makeMoveSetTime(b, log, logging, simgames, *ztable, maxdepth, numnodes, time);
            log.close();
            i++;
            cout<<b.GetFen()<<endl;
            string humanMove;
            bool moveMade = false;
            while (!moveMade) {
                cin>>humanMove;
                moveMade = b.MakeMoveFromUci(humanMove);
                if (!moveMade) {
                    cout<<"bad move"<<endl;
                }
            }
            simgames<<humanMove<<endl;
        }
        cout<<endl;
    }
}