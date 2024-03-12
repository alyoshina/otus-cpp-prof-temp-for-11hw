Asynchronous server that performs intersection and symmetrical difference operations.

Start:
    join_server <port>

Database contains tables:
    A
    B

The tables structure is the same:
    id INTEGER PRIMARY KEY
    name TEXT

Protocol:
    INSERT table id name
    TRUNCATE table
    INTERSECTION
    SYMMETRIC_DIFFERENCE

Examples:
    INSERT A 0 lean
    INSERT A 0 understand
    INSERT A 1 sweater
    INSERT A 2 frank
    INSERT A 3 violation
    INSERT A 4 quality
    INSERT A 5 precision
    INSERT B 3 proposal
    INSERT B 4 example
    INSERT B 5 lake
    INSERT B 6 flour
    INSERT B 7 wonder
    INSERT B 8 selection

    INTERSECTION

    SYMMETRIC_DIFFERENCE

    TRUNCATE A
    