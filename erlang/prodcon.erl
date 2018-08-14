-module(prodcon).
-export([start/0, consumer/1, producer/2]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
producer(_, 0) ->
    io:format("~n~nproducer finished~n"),
    true;
producer(Server, N) ->
    Server ! {N},
    producer(Server, N-1).
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
consumer_fn(Parent, N) ->
	io:format("~w~n", [N]),
        if
            1 == N -> Parent ! {N};
            true -> consumer(Parent)
        end.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
consumer(Parent) ->
    receive
        {N} -> consumer_fn(Parent, N);
        stop -> true
    end.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
start() ->
    Consumer = spawn(prodcon, consumer, [self()]),
    Producer = spawn(prodcon, producer, [Consumer, 10]),
    io:format("Producer = ~p Consumer = ~p", [Producer, Consumer]),
    io:format("~n~nstart finished~n"),
    receive {N} ->
	io:format("exiting~n"),
        Consumer ! stop
    end.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

