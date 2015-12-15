DROP TABLE participation;
DROP TABLE current_rating;
DROP TABLE current_title;
DROP TABLE chessplayer;
DROP TABLE tournament;
DROP TABLE city;
DROP TABLE country;
DROP TABLE title;

CREATE TABLE title (
  title_id NUMBER(2)
    PRIMARY KEY,
  name VARCHAR2(50)
    NOT NULL
);

CREATE TABLE country (
  country_id NUMBER(4)
    PRIMARY KEY,
  name VARCHAR2(50)
    NOT NULL
);

CREATE TABLE city (
  city_id NUMBER(4)
    PRIMARY KEY,
  name VARCHAR2(50)
    NOT NULL,
  country_id NUMBER(4)
    NOT NULL
    REFERENCES country(country_id)
    ON DELETE CASCADE
);

CREATE TABLE chessplayer (
  chessplayer_id NUMBER(4)
    PRIMARY KEY,
  firstname VARCHAR2(50)
    NOT NULL,
  lastname VARCHAR2(50)
    NOT NULL,
  initial_rating NUMBER(4)
    DEFAULT 1000
    NOT NULL
    CHECK(initial_rating > 0),
  country_id NUMBER(4)
    REFERENCES country(country_id) 
    ON DELETE CASCADE
);

CREATE TABLE current_rating (
  chessplayer_id NUMBER(4)
    PRIMARY KEY
    REFERENCES chessplayer(chessplayer_id)
    ON DELETE CASCADE,
  value NUMBER(4)
    NOT NULL
    CHECK(value > 0)
);

CREATE TABLE current_title (
  chessplayer_id NUMBER(4)
    PRIMARY KEY
    REFERENCES chessplayer(chessplayer_id)
    ON DELETE CASCADE,
  title_id NUMBER(2)
    NOT NULL
    REFERENCES title(title_id)
    ON DELETE CASCADE
);

CREATE TABLE tournament (
  tournament_id NUMBER(4)
    PRIMARY KEY,
  name VARCHAR2(50)
    NOT NULL,
  start_date DATE,
  end_date DATE,
  city_id NUMBER(4)
    REFERENCES city(city_id)
    ON DELETE CASCADE,
  CHECK(start_date < end_date)
);

CREATE TABLE participation (
  chessplayer_id NUMBER(4)
    NOT NULL
    REFERENCES chessplayer(chessplayer_id)
    ON DELETE CASCADE,
  tournament_id NUMBER(4)
    NOT NULL
    REFERENCES tournament(tournament_id)
    ON DELETE CASCADE,
  rating_change NUMBER(4)
    DEFAULT(0)
    NOT NULL,
  place NUMBER(4)
    NOT NULL
    CHECK(place > 0),
  PRIMARY KEY (tournament_id, chessplayer_id)
);

