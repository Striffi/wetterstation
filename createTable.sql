create table weathervalues (

    key bigserial primary key,
    time timestamp,
    sensor varchar(20),
    measurement decimal

);
