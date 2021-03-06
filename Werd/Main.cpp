#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <cctype>

bool vector_contains_word( const std::vector<std::string>& vec,const std::string& word )
{
	for( const auto& w : vec )
	{
		if( w == word )
		{
			return true;
		}
	}
	return false;
}

std::vector<int> fill_buckets( const std::string& word )
{
	std::vector<int> buckets( 26,0 );
	for( char c : word )
	{
		buckets[c - 'a']++;
	}
	return buckets;
}

int score_match( const std::string& word1,const std::string& word2 )
{
	const auto buckets1 = fill_buckets( word1 );
	const auto buckets2 = fill_buckets( word2 );

	int score = 0;
	for( int i = 0; i < 26; i++ )
	{
		score += std::min( buckets1[i],buckets2[i] );
	}

	for( int i = 0; i < 5; i++ )
	{
		if( word1[i] == word2[i] )
		{
			score++;
		}
	}

	return score;
}

void loadFreqFile( std::vector<std::string>& freq_five_words ) {
	std::ifstream freqFile( "20k.txt" );
	for (std::string line; std::getline(freqFile, line); ) {
		if (line.length() == 5) {
			freq_five_words.push_back( line );
		}
	}
}

void sortFreqList( std::vector<std::string>& freq_five_words ) {
	std::sort( freq_five_words.begin(), freq_five_words.end());
}

bool isInFreqList( const std::string searchTerm, const std::vector<std::string>& freq_five_words ) {
	std::vector<std::string>::iterator it;

	return (std::find( freq_five_words.begin(), freq_five_words.end(), searchTerm ) != freq_five_words.end());
}

int main()
{
	std::vector<std::string> five_words;
	std::vector<std::string> freqList;
	std::string testWord;
	
	{
		std::ifstream five_word_file( "sgb-words.txt" );
		for( std::string line; std::getline( five_word_file,line ); )
		{
			if( line.empty() )
			{
				continue;
			}
			five_words.push_back( line );
		}
	}

	loadFreqFile( freqList );
	sortFreqList( freqList );

	std::mt19937 rng( std::random_device{}() );
	std::uniform_int_distribution<int> dist( 0,five_words.size() - 1 );

	int five_words_index = dist( rng );

	while (!isInFreqList( five_words[five_words_index], freqList )) {
		five_words_index = dist( rng );
	}

	const std::string target = five_words[five_words_index];



	while( true )
	{
		std::cout << "Guess a five letter word: ";
		std::string guess;
		std::getline( std::cin,guess );

		for( auto& c : guess )
		{
			c = std::tolower( c );
		}

		if( guess.size() != 5 )
		{
			std::cout << "I said FIVE letter word, asshole." << std::endl;
			continue;
		}

		if( !vector_contains_word( five_words,guess ) )
		{
			std::cout << guess << "? That isn't even a fucking word." << std::endl;
			continue;
		}

		const int score = score_match( guess,target );
		if( score == 10 )
		{
			std::cout << "You did it. Wow." << std::endl;
			break;
		}
		else
		{
			std::cout << "Nah that ain't it. I'll give you " << score << " points for effort though (retard)." << std::endl;
			continue;
		}
	}

	std::cin.clear();
	std::cin.ignore( std::numeric_limits<std::streamsize>::max(),'\n' );
	std::cin.get();
	return 0;
}