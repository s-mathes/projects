-- QUESTION 1 --
SELECT AVG(budget) FROM movies.movies;

-- QUESTION 2 --
SELECT original_title FROM movies.movies 
	INNER JOIN movies.movie_production_country ON movies.movie_production_country.movieid = movies.movies.id
WHERE movies.movie_production_country.iso_3166_1 = 'US';

-- QUESTION 3 --
SELECT original_title, revenue FROM movies.movies ORDER BY movies.movies.revenue DESC LIMIT 5;

-- QUESTION 4 -- 
SELECT original_title FROM movies.movies
	INNER JOIN movies.movie_genre ON movies.movies.id = movies.movie_genre.movieid
    INNER JOIN movies.genres ON movies.movie_genre.genreid = movies.genres.id
WHERE movies.genres.name = 'Science Fiction' OR movies.genres.name = "Mystery";

-- QUESTION 5 -- 
SELECT original_title, popularity FROM movies.movies WHERE popularity > (SELECT AVG(popularity) FROM movies.movies);
