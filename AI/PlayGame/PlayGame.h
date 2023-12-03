#include "../Search/Search.h"
using namespace Search;

namespace PlayGame {
    string simgamefile = "../../UCI/simgames.txt";

    bool MakeMoveSetDepth(Bitboard &b, ofstream &log, bool &logging, ofstream &simgames, ZTable &ztable, int &numnodes, int depth=1) {
        if (logging) log<<"starting set-depth search for depth: "<<depth<<"and FEN: "<<b.GetFen()<<endl;
        int bestMove = 0;
        // bestMove = bestMoveAtDepth(b, log, logging, ztable, depth);
        for (int i=0; i<depth+1; i++) bestMove = BestMoveAtDepth(b, log, logging, ztable, numnodes, i);

        if (bestMove == 0) {
            if (logging) log<<"game is over"<<endl;
            return false;
        }

        if (logging) {log<<"making move "<<GetMoveUci(bestMove)<<endl;}
        cout<<GetMoveUci(bestMove)<<endl;
        bool movemade = b.MakeMove(bestMove);

        if (!movemade) {
            cout<<"ERROR FOR FEN: "<<b.GetFen();
            cout<<" with num: "<<bestMove;
            cout<<"or uci move: "<<GetMoveUci(bestMove)<<endl<<endl;
        } else {
            simgames<<GetMoveUci(bestMove)<<endl;
        }
        return true;
    }

    bool MakeMoveSetTime(Bitboard &b, ofstream &log, bool &logging, ofstream &simgames, ZTable &ztable, int &numnodes, int &maxdepth, int expected_time) {
        if (logging) log<<"starting set-time search for time: "<<time<<"and FEN: "<<b.GetFen()<<endl;
        int bestMove = 0;
        // bestMove = bestMoveAtDepth(b, log, logging, ztable, depth);
        long time_taken = 0;
        long time_elapsed = 0;
        int depth = 0;

        while (time_taken+3*time_elapsed < expected_time*1000) { //TODO: change equation
            long start = clock();
            bestMove = BestMoveAtDepth(b, log, logging, ztable, numnodes, depth);
            long end = clock();
            time_elapsed = end-start;
            // cout<<"time elapsed: "<<time_elapsed<<endl;
            time_taken+=time_elapsed;
            depth++;
        }
        maxdepth = depth-1;

        if (bestMove == 0) {
            if (logging) log<<"game is over"<<endl;
            return false;
        }

        if (logging) {log<<"making move "<<GetMoveUci(bestMove)<<endl;}
        cout<<GetMoveUci(bestMove)<<endl;
        bool movemade = b.MakeMove(bestMove);
        if (!movemade) {
            cout<<"ERROR FOR FEN: "<<b.GetFen();
            cout<<" with num: "<<bestMove;
            cout<<"or uci move: "<<GetMoveUci(bestMove)<<endl<<endl;
        } else {
            simgames<<GetMoveUci(bestMove)<<endl;
        }
        return true;
    }

    void playgame_setdepth(int depth, string fen, bool logging) {
        srand(5);
        Bitboard b(fen);
        ofstream log;
        ofstream simgames;
        simgames.open(simgamefile);
        simgames<<fen<<endl;
        ZTable* ztable = new ZTable;
        
        int i = 0;
        bool isGameOngoing = true;
        while (isGameOngoing  && i < 250) {
            int numnodes = 0;
            long start = clock();
            log.open("logs\\log_"+to_string(i)+".md");
            isGameOngoing = MakeMoveSetDepth(b, log, logging, simgames, *ztable, numnodes, depth);
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
        simgames<<fen<<endl;
        ZTable* ztable = new ZTable;

        int i = 0;
        bool isGameOngoing = true;
        while (isGameOngoing && i < 250) {
            int numnodes = 0;
            long start = clock();
            int maxdepth = 0;
            log.open("logs\\log_"+to_string(i)+".md");
            isGameOngoing = MakeMoveSetTime(b, log, logging, simgames, *ztable, numnodes, maxdepth, time);
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
        simgames<<fen<<endl;
        ZTable* ztable = new ZTable;
        
        int i = 0;
        bool isGameOngoing = true;
        while (isGameOngoing  && i < 250) {
            int numnodes = 0;
            long start = clock();
            log.open("logs\\log_"+to_string(i)+".md");
            isGameOngoing = MakeMoveSetDepth(b, log, logging, simgames, *ztable, numnodes, depth);
            log.close();
            i++;
            long end = clock();
            cout<<i<<"-searched "<<numnodes<<" nodes in "<<end-start<<"ms"<<endl;
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
        simgames<<fen<<endl;
        ZTable* ztable = new ZTable;
        
        int i = 0;
        bool isGameOngoing = true;
        while (isGameOngoing  && i < 250) {
            int numnodes = 0;
            long start = clock();
            int maxdepth = 0;
            log.open("logs\\log_"+to_string(i)+".md");
            isGameOngoing = MakeMoveSetTime(b, log, logging, simgames, *ztable, numnodes, maxdepth, time);
            log.close();
            i++;
            long end = clock();
            cout<<i<<"-searched "<<numnodes<<" nodes in "<<end-start<<"ms up to depth "<<maxdepth<<endl;
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