#include <iostream>


class White{
        private:
                int mode;
        public:
        White(int Mode = 1);


        
        void Bishoop(int x , int y , int c );
        void King(int x , int y , int c);
        void Knight(int x , int y , int c );
        void Pawn(int x , int y , int c);
        void Queen(int x , int y , int c );
        void Rook(int x , int y , int c );
        void Empty(int x , int y , int c);
};



class Black{
    private:
        int mode;
    public:
        Black(int Mode = 1);
        void Bishoop(int x , int y , int c );
        void King(int x , int y , int c);
        void Knight(int x , int y , int c );
        void Pawn(int x , int y , int c);
        void Queen(int x , int y , int c );
        void Rook(int x , int y , int c );
        void Empty(int x , int y , int c);
};







