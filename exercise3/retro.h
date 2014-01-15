#pragma once

enum Op { INSERT, DELETE };

class Retro {

    public:
        virtual ~Retro() {};

        /**
         * Insert the given operation at time m, where m is
         * the number of operations in the timeline, i.e. in
         * the present (rather than in the past).
         */
        virtual void Insert(Op o, int key) = 0;

        /**
         * Insert the given operation at time t, between the
         * current time t and time t+1.
         */
        virtual void Insert(int t, Op o, int key) = 0;

        /**
         * Delete the operation at time t, making the operation
         * at time t+1 (if it exists) the new operation at time
         * t. If t = (m-1), where m is the number of operations
         * in the timeline, we are deleting the last operation,
         * effectively making a classic Ctrl+Z (undo).
         */
        virtual void Delete(int t) = 0;

        /**
         * Make a query (predecessor search) for the given key
         * at time m, i.e. in the present.
         */
        virtual int Query(int key) = 0;

        /**
         * Make a query (predecessor search) for the given key
         * at the given time.
         */
        virtual int Query(int t, int key) = 0;

};