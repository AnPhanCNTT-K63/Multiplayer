#pragma once
#include<string>
enum Suits
{
	HEARTS = 3,
	DIAMONDS,
	CLUBS,
	SPADES
};

class Card
{
private:
	string ranks; // 2->A
	int suits;	  // clubs, diamonds, hearts, spades
	int value;
	string path; // where the card stored in file 'src'

	// Current displayed texture
	SDL_Texture *texture = NULL;

	// where cards will appear
	SDL_Rect destinationRect;

	int x;
	int y;
	int width;
	int height;

public:
	Card()
	{
		this->x = 0;
		this->y = 560;
		this->width = 132;
		this->height = 192;

		setDestinationRect();
	}

	SDL_Texture *getTexture()
	{
		return this->texture;
	}

	void blackColor()
	{
		SDL_SetTextureColorMod(texture, 120, 120, 120);
	}

	void normalColor()
	{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
	}

	void setTexture(SDL_Texture *texture)
	{
		this->texture = texture;
	}

	SDL_Rect *getDestinationRect()
	{
		return &(this->destinationRect);
	}
	void setDestinationRect()
	{
		this->destinationRect = {this->x,
								 this->y,
								 this->width,
								 this->height};
	}
	void setRanks(string ranks)
	{
		this->ranks = ranks;
	}
	string getRanks()
	{
		return ranks;
	}
	void setPath(string rank, int suit, string themeCard)
	{
		this->path = "src/cards/";
		this->path += themeCard;
		this->path += rank;
		this->path += "-";

		switch (suits)
		{
		case 3:
			this->path += "H";
			break;
		case 4:
			this->path += "D";
			break;
		case 5:
			this->path += "C";
			break;
		case 6:
			this->path += "S";
			break;
		}
		this->path += ".png";
	}
	string getPath()
	{
		return path;
	}
	void setSuits(int suits)
	{
		this->suits = suits;
	}
	int getSuits()
	{
		return suits;
	}
	void setValue(string rank)
	{
		if (rank == "J")
		{
			this->value = 11;
		}
		else if (rank == "Q")
		{
			this->value = 12;
		}
		else if (rank == "K")
		{
			this->value = 13;
		}
		else if (rank == "A")
		{
			this->value = 14;
		}
		else if (rank == "2")
		{
			this->value = 15;
		}
		else
		{
			this->value = stoi(rank);
		}
	}
	int getValue()
	{
		return value;
	}
	void setX(int x)
	{
		this->x = x;
		setDestinationRect();
	}
	void setY(int y)
	{
		this->y = y;
		setDestinationRect();
	}
	void setWidth(int width)
	{
		this->width = width;
		setDestinationRect();
	}
	void setHeight(int height)
	{
		this->height = height;
		setDestinationRect();
	}
	int getX()
	{
		return this->x;
	}
	int getY()
	{
		return this->y;
	}
	int getWidth()
	{
		return this->width;
	}
	int getHeight()
	{
		return this->height;
	}
};

class PlayingCards
{
private:
	static const string ranks[13];
	static const int suits[4];
	vector<Card> playingCards;

public:
	// Constructor
	PlayingCards()
	{
		this->createPlayingCards();
		this->shufflePlayingCards();
	}
	vector<Card> getPlayingCards()
	{
		return playingCards;
	}
	void setPlayingCards(vector<Card> playingCards)
	{
		this->playingCards = playingCards;
	}
	// Helper function to generate playingcards
	void createPlayingCards()
	{
		for (int j = 0; j < 13; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				Card card; // declare a new Card object inside the loop
				card.setRanks(ranks[j]);
				card.setSuits(suits[k]);
				card.setValue(ranks[j]);
				card.setPath(card.getRanks(), card.getSuits(), themeCard);
				card.setTexture(loadTexture(card.getPath()));

				playingCards.push_back(card);
			}
		}
	}
	void shufflePlayingCards()
	{
		for (int i = 0; i < playingCards.size(); i++)
		{
			int j = rand() % 52; // generate a random number between 0 - 51
			Card temp = playingCards[i];
			playingCards[i] = playingCards[j];
			playingCards[j] = temp;
		}
	}
	Card get1Card()
	{
		Card lastCard = playingCards.back();
		playingCards.pop_back();
		return lastCard;
	}
};

const string PlayingCards::ranks[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
const int PlayingCards::suits[4] = {CLUBS, DIAMONDS, HEARTS, SPADES};