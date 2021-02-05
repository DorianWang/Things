import java.util.ArrayList;
import java.util.Random;

/**
 * Deck models a deck of 52 Anglo-American playing cards.
 * 
 * @author Lynn Marshall and Dorian Wang
 * @version 1.1.1 May 24, 2016
 *
 */
public class Deck
{
    /** 
     * The cards are stored in a linked-list implementation of the
     * List collection.
     */
    private ArrayList<Card> cards;
    
    /** Lowest ranking card (the ace). */
    private static final int ACE = 1;
    
    /** Highest ranking card (the king). */
    private static final int KING = 13;
    
    /** 
     * Total number cards in the deck (4 suits, each with 13 cards of 
     * different ranks).
     */ 
    private static final int TOTAL_CARDS = 52;
    
    /** 
     * Some constants that define the Strings for the various suits.
     */ 
    private static final String HEARTS = "hearts";
    private static final String DIAMONDS = "diamonds";
    private static final String CLUBS = "clubs";
    private static final String SPADES = "spades";

    /**
     * Constructs a new, unshuffled deck containing 52 playing cards.
     */
    public Deck()
    {              
        cards = new ArrayList<Card>();
        buildSuit(HEARTS);
        buildSuit(DIAMONDS);
        buildSuit(CLUBS);
        buildSuit(SPADES);
    }
    
    /**
     * Creates the 13 cards for the specified suit, and adds them
     * to this deck.
     * @param suit the suit to be created
     */
    private void buildSuit(String suit)
    {
        for (int i = 0; i < 13; i++){
            cards.add(new Card(suit, i + 1));
        }
    }
 
    /**
     * Shuffles this deck of cards. Swaps two random cards 100,000 times.
     */
    public void shuffle()
    {   
        //It isn't required to actually swap 100,000 times. 1000 swaps would be more than
        //sufficient. Woo math!
        Random newRand = new java.util.Random();
        for (int i = 0; i < 100000; i++){
            int rand1 = newRand.nextInt(TOTAL_CARDS);
            int rand2 = newRand.nextInt(TOTAL_CARDS);
            Card tempCard = cards.get(rand1);
            cards.set(rand1, cards.get(rand2));
            cards.set(rand2, tempCard);
        }
        
    }
 
    /**
     * Removes a card from this deck.
     * @return the card removed
     */
    public Card dealCard()
    {
        if (isEmpty() == false){
            Card tempCard = cards.get(0);
            cards.remove(0);
            return tempCard;
        }
        return null;
    }
 
    /**
     * Determines if this deck is empty.
     * @return true if the deck is empty, false otherwise
     */
    public boolean isEmpty()
    {
        return cards.isEmpty();
    }
  
    /**
     * Returns the number of cards that are currently in the deck.
     * @return the number of cards in the deck.
     */
    public int size()
    {
        return cards.size();
    }
}

















