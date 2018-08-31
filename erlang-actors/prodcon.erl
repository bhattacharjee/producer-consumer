-module(prodcon).
-export([start/0, consumer/1, producer/2]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
producer(_, 0) ->                      % termination condition for recursion
    io:format("~n~nproducer finished~n"),
    true;
producer(TheConsumer, N) ->
    TheConsumer ! {N},                 % send N to the consumer
    producer(TheConsumer, N-1).        % tail recursion with N-1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
consumer_fn(Parent, N) ->
    io:format("~w~n", [N]),
    if
        1 == N -> Parent ! {N};        % if N = 1, send completion msg to start 
        true -> consumer(Parent)       % else consume again
    end.

consumer(Parent) ->
    receive                            % receive a message from producer
        {N} -> consumer_fn(Parent, N); % call consumer function if it is a num
        stop -> true                   % stop if it is a stop message
    end.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
start() ->
    Consumer = spawn(prodcon, consumer, [self()]),
    Producer = spawn(prodcon, producer, [Consumer, 10]),
    io:format("Producer = ~p Consumer = ~p", [Producer, Consumer]),
    io:format("~n~nstart finished~n"),
    receive {N} ->                     % wait for consumer's completion msg
	io:format("exiting~n"),
        Consumer ! stop                % ask the consumer to stop
    end.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
