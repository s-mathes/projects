import pymysql
from prettytable import PrettyTable
import cv2
import csv
import pandas as pd
import ast
from datetime import datetime

def createRelations():
	conn = pymysql.connect(host="localhost", port=3306, user="root")

    #cursor communicates with mySQL server
	curr = conn.cursor() 
	
	# "DROP DATABASE movies" if it exists
#	sql = 'DROP DATABASE movies'
#	curr.execute(sql)
	sql = 'CREATE DATABASE movies'
	curr.execute(sql)
	conn = pymysql.connect(host="localhost", port=3306, user="root", database="movies")
	curr = conn.cursor()
	
	# make tables
	sql = 'CREATE TABLE movies (budget INT, homepage VARCHAR(255),id INT PRIMARY KEY, original_language VARCHAR(10), original_title VARCHAR(255), overview VARCHAR(2048), popularity INT, release_date DATE, revenue BIGINT, runtime INT, status VARCHAR(127), tagline VARCHAR(255), title VARCHAR(255), vote_average INT, vote_count INT)'
	curr.execute(sql)
	sql = 'CREATE TABLE genres (name VARCHAR(255), id INT PRIMARY KEY)'
	curr.execute(sql)
	sql = 'CREATE TABLE movie_genre (id INT AUTO_INCREMENT PRIMARY KEY, genreid INT, movieid INT, FOREIGN KEY(genreId) REFERENCES genres(id), FOREIGN KEY(movieid) references movies(id) )'
	curr.execute(sql)
	sql = 'CREATE TABLE production_countries (iso_3166_1 VARCHAR(5) PRIMARY KEY, name VARCHAR(255))'
	curr.execute(sql)
	sql = 'CREATE TABLE movie_production_country ( id INT AUTO_INCREMENT PRIMARY KEY, iso_3166_1 VARCHAR(5), movieid INT, FOREIGN KEY(iso_3166_1) REFERENCES production_countries(iso_3166_1), FOREIGN KEY(movieid) REFERENCES movies(id) )'
	curr.execute(sql)
	sql = 'CREATE TABLE spoken_languages (iso_639_1 VARCHAR(5) PRIMARY KEY, name VARCHAR(255))'
	curr.execute(sql)
	sql = 'CREATE TABLE movie_spoken_language ( id INT AUTO_INCREMENT PRIMARY KEY, iso_639_1 VARCHAR(5), movieid INT, FOREIGN KEY(iso_639_1) REFERENCES spoken_languages(iso_639_1), FOREIGN KEY(movieid) REFERENCES movies(id) )'
	curr.execute(sql)
	sql = 'CREATE TABLE keywords (id INT, name VARCHAR(255), PRIMARY KEY(id) )'
	curr.execute(sql)
	sql = 'CREATE TABLE movie_keywords (id INT PRIMARY KEY AUTO_INCREMENT, keywordid INT, movieid INT, FOREIGN KEY(keywordid) REFERENCES keywords(id), FOREIGN KEY(movieid) REFERENCES movies(id) )'
	curr.execute(sql)
	sql = 'CREATE TABLE production_companies (id INT PRIMARY KEY, name VARCHAR(255) )'
	curr.execute(sql)
	sql = ' CREATE TABLE movie_production_companies (id INT AUTO_INCREMENT PRIMARY KEY, producerid INT, movieid INT, FOREIGN KEY(producerid) REFERENCES production_companies(id), FOREIGN KEY(movieid) REFERENCES movies(id) ) '
	curr.execute(sql)


	sql = 'SHOW TABLES'
	curr.execute(sql)

	
# The second method should insert and parse the data from the provided CSV file into your relations
def parseData(inputFile,outputFile):
	conn = pymysql.connect(host="localhost", port=3306, user="root",passwd="togepi89")

    #cursor communicates with mySQL server
	curr = conn.cursor() 
	
	# "DROP DATABASE movies" if it exists
	conn = pymysql.connect(host="localhost", port=3306, user="root",passwd="togepi89", database="movies")
	curr = conn.cursor()
	
	# read csv file into a dataframe
	csv_file = open(inputFile,'r',encoding="utf8")
	data= csv.reader(csv_file) #, names=['budget','genres','homepage','id','keywords','original_language','original_title','overview','popularity','production_companies','production_country','release_date','revenue','runtime','spoken_languages','status','tagline','title','vote_average','vote_count'])
	
	# prep SQL statements
	add_movie = 'INSERT INTO movies (budget, homepage, id, original_language, original_title, overview, popularity, release_date, revenue, runtime, status, tagline, title, vote_average, vote_count) VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)'
	add_genre = 'INSERT INTO genres (id,name) VALUES (%s,%s)'
	add_movie_keywords = 'INSERT INTO movie_keywords (keywordid, movieid) VALUES (%s,%s)'
	add_movie_genre = 'INSERT INTO movie_genre (genreid, movieid) VALUES (%s, %s)'
	add_keywords = 'INSERT INTO keywords (id, name) VALUES (%s,%s)'
	add_movie_production_company = 'INSERT INTO movie_production_companies (producerid, movieid) VALUES (%s,%s)' 
	add_production_company = 'INSERT INTO production_companies (id, name) VALUES (%s,%s) ON DUPLICATE KEY UPDATE id=%s'
	add_production_country = 'INSERT INTO production_countries (iso_3166_1, name) VALUES (%s, %s)'
	add_movie_production_country = 'INSERT INTO movie_production_country (iso_3166_1, movieid) VALUES (%s,%s)'
	add_spoken_language = 'INSERT INTO spoken_languages (iso_639_1,name) VALUES (%s,%s)'
	add_movie_spoken_language = 'INSERT INTO movie_spoken_language (iso_639_1, movieid) VALUES (%s,%s)'
	
	attributes = next(data)
	for row in data:
		try: # create a list of rows for 'movies' 
			dateObj = datetime.strptime(row[11], '%m/%d/%Y')
			newDateFormat = dateObj.strftime('%Y-%m-%d')
			values = (row[0],row[2],row[3],row[5],row[6],row[7],row[8],newDateFormat,row[12],row[13],row[15],row[16],row[17],row[18],row[19])
		except:
			continue
		try:
			curr.execute(add_movie,values)
			conn.commit()
		except pymysql.Error as err:
			# print(err)
			continue
		
		 # create a list of rows for 'genres'
		genres = eval(row[1])
		for key in genres:
			genre = (key['id'], key['name'])
			movie_genre = (key['id'], row[3])
			try: # set up table 'genres'
				curr.execute(add_genre, genre)
				conn.commit()
			except pymysql.Error as err:
				pass
			try: #set up table 'movie_genres'
				curr.execute(add_movie_genre, movie_genre)
				conn.commit()
			except pymysql.Error as err:
				pass
				
		# create a list of rows for 'keywords'
		keywords = eval(row[4])
		for key in keywords:
			keyword = (key['id'], key['name'])
			movie_keyword = (key['id'], row[3])
			try: # set up table 'keywords'
				curr.execute(add_keywords, keyword)
				conn.commit()
			except pymysql.Error as err:
				pass
			try: #set up table 'movie_keywords'
				curr.execute(add_movie_keywords, movie_keyword)
				conn.commit()
			except pymysql.Error as err:
				pass

		#create a list of rows for 'production_companies'
		production_companies = eval(row[9])
		for key in production_companies:
			production_company = (key['id'], key['name'], key['id'])
			movie_production_company = (key['id'], row[3])
			curr.execute(add_production_company, production_company)
			conn.commit()
			try: # set up table 'production_company'
				curr.execute(add_production_company, production_company)
				conn.commit()
			except pymysql.Error as err:
				pass
			try: #set up table 'movie_production_company'
				curr.execute(add_movie_production_company, movie_production_company)
				conn.commit()
			except pymysql.Error as err:
				pass
				
		# create a list of rows for 'production_countries'
		production_countries = eval(row[10])
		for key in production_countries:
			production_country = (key['iso_3166_1'], key['name'])
			movie_production_country = (key['iso_3166_1'], row[3])
			try: # set up table 'production_country'
				curr.execute(add_production_country, production_country)
				conn.commit()
			except pymysql.Error as err:
				pass
			try: #set up table 'movie_production_country'
				curr.execute(add_movie_production_country, movie_production_country)
				conn.commit()
			except pymysql.Error as err:
				print(err)
				pass

		# create a list of rows for 'spoken_languages'
		spoken_languages = eval(row[14])
		for key in spoken_languages:
			spoken_language = (key['iso_639_1'], key['name'])
			movie_spoken_language = (key['iso_639_1'], row[3])
			try: # set up table 'spoken_language'
				curr.execute(add_spoken_language, spoken_language)
				conn.commit()
			except pymysql.Error as err:
				pass
			try: #set up table 'movie_spoken_language'
				curr.execute(add_movie_spoken_language, movie_spoken_language)
				conn.commit()
			except pymysql.Error as err:
				pass
				
			
def main():
	createRelations()
	parseData("tmdb_5000_movies.csv", "NULL")


if __name__ == '__main__':
	main()


