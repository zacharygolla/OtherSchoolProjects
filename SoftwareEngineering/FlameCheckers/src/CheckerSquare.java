import javax.swing.*;
import javax.swing.border.Border;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;


// This class creates the individual squares that make up the the checkers board. Creating individual squares should
// make mobility on the board with the pieces a lot more easy to do. The count starts on the top  left with 'val' = 1,
// and goes down to 'val' = 64.
public class CheckerSquare extends JPanel {

    public static final int WIDTH = 70; //width of each tile
    public static final int HEIGHT = 70; //height of each tile, most likely same as width so its a square
    public static  boolean NIGHT = false; // sets the value of the night mode to false which will then be updated by
                                          // the options chose

    public static int squareVal = 1; //variable to reference unique id for each tile


    public int val; // current tile you're sitting on ( 1- 64), runs from left to right in numeration value
    private int x; //x coordinate
    private int y; //y coordinate
    private int height; // tile height
    private int width; //tile width
    private boolean night;// stores the value of the night mode
    public String hasPiece = "None"; // tells the space if it has a piece
    private boolean kingStatus;
    private JLabel pieceLabel;
    public int moves[] = new int[4];

    // Constructor that will take in x and y to determine what coordinate you are on and if the night value was used
    public CheckerSquare( int x, int y, boolean n ) {


        this.val = squareVal++;
        this.x = x;
        this.y = y;
        width = WIDTH;
        height = HEIGHT;
        night = NIGHT;

        if(this.val == 2 || this.val == 4 || this.val == 6 || this.val == 8 ||
                this.val == 9 || this.val == 11 || this.val == 13 || this.val == 15 ||
                this.val == 18 || this.val == 20 || this.val == 22 || this.val == 24) {
            this.add(this.pieceLabel = new JLabel(new ImageIcon("./3-Checkers-Photo-5.png")));
            this.hasPiece = "Red";
            this.kingStatus = false;
        }
        else if (this.val == 41 || this.val == 43 || this.val == 45 || this.val == 47 ||
                this.val == 50 || this.val == 52 || this.val == 54 || this.val == 56 ||
                this.val == 57 || this.val == 59 || this.val == 61 || this.val == 63) {
            this.add(this.pieceLabel = new JLabel(new ImageIcon("./3-Checkers-Photo-6.png")));
            this.hasPiece = "Black";
            this.kingStatus = false;
        }
    }


    // Creates an array of the squares val's that are available to be moved to if a square
    // with a piece on it is selected
    public void availableMoves(int val, String color, boolean king){
        if(color.equals("Black")){
            if(!king){
                if(val == 2 || val == 4 || val == 6 || val == 8){
                    king = true;
                }
                else if(val == 57 || val == 41 || val == 25 || val == 9){
                    moves[0] = (val - 7);
                    moves[1] = 0;
                    moves[2] = 0;
                    moves[3] = 0;

                }
                else if(val == 56 || val == 40 || val == 24){
                    moves[0] = (val - 9);
                    moves[1] = 0;
                    moves[2] = 0;
                    moves[3] = 0;
                }
                else{
                    moves[0] = (val - 9);
                    moves[1] = (val - 7);
                    moves[2] = 0;
                    moves[3] = 0;
                }
            }
            else{
                System.out.println("Black kings move set");
            }
        }
        else if(color.equals("Red")){
            if(!king){
                if(val == 57 || val == 59 || val == 61 || val == 63){
                    king = true;
                }
                else if(val == 8 || val == 24 || val == 40 || val == 56){
                    moves[0] = (val + 7);
                    moves[1] = 0;
                    moves[2] = 0;
                    moves[3] = 0;

                }
                else if(val == 9 || val == 25 || val == 41){
                    moves[0] = (val + 9);
                    moves[1] = 0;
                    moves[2] = 0;
                    moves[3] = 0;
                }
                else{
                    moves[0] = (val + 9);
                    moves[1] = (val + 7);
                    moves[2] = 0;
                    moves[3] = 0;
                }
            }
            else{
                System.out.println("Red kings move set");
            }
        }
    }


    //Fills the square with the appropriate color at the appropriate value and coordinate. Paints the board in the
    // appropriate colors depending if night mode was selected or not.
    public void color(GameBoard gb)
    {
        if (night == false)
        {

            if ((val > 0 && val <9 && val % 2 == 0) || (val > 8 && val <17 && val % 2 == 1) ||  (val > 16 && val <25 && val % 2 == 0)
                    || (val > 24 && val <33 && val % 2 == 1) || (val > 32 && val <41 && val % 2 == 0) || (val > 40 && val <49 && val % 2 == 1)
                    || (val > 48 && val <57 && val % 2 == 0) || (val > 56 && val <65 && val % 2 == 1))
            {

                //Red squares are the only squares the game are played on
                this.setBackground(Color.RED);
                this.addMouseListener(new MouseListener() {

                    boolean isHighlighted = false;
                    @Override
                    public void mouseClicked(MouseEvent e) {
                        // this makes the square stayed highlighted when you click it and unhighlight when you click it again.
                        // then if the square has a piece on it, it will cal the availableMoves function to high light the
                        // available moves for that piece, then when the piece is clicked again to unhighlight it removes the
                        // available moves squares highlighting
                        if (isHighlighted) {
                            setBorder(BorderFactory.createLineBorder(Color.red, 3));
                            gb.removeAvailableMoves(moves, "Red");
                        }
                        else if(!isHighlighted && (hasPiece.equals("Black") || hasPiece.equals("Red"))) {
                            setBorder(BorderFactory.createLineBorder(Color.MAGENTA, 3));
                            if(hasPiece.equals("Black")) {
                                availableMoves(val, "Black", kingStatus);
                                gb.showAvailableMoves(moves);
                            }
                            else if(hasPiece.equals("Red")){
                                availableMoves(val, "Red", kingStatus);
                                gb.showAvailableMoves(moves);
                            }
                        }
                        isHighlighted = !isHighlighted;
                    }

                    @Override
                    public void mousePressed(MouseEvent e) { }

                    @Override
                    public void mouseReleased(MouseEvent e) { }

                    @Override
                    public void mouseEntered(MouseEvent e) { }

                    @Override
                    public void mouseExited(MouseEvent e) { }
                });



            }
            else {
                this.setBackground(Color.ORANGE);
            }

        }
        else {
                if ((val > 0 && val <9 && val % 2 == 0) || (val > 8 && val <17 && val % 2 == 1) ||  (val > 16 && val <25 && val % 2 == 0)
                        || (val > 24 && val <33 && val % 2 == 1) || (val > 32 && val <41 && val % 2 == 0) || (val > 40 && val <49 && val % 2 == 1)
                        || (val > 48 && val <57 && val % 2 == 0) || (val > 56 && val <65 && val % 2 == 1))
                {

                    //Dark grey squares are the only squares that are played on
                    this.setBackground(Color.DARK_GRAY);
                    this.addMouseListener(new MouseListener() {

                        boolean isHighlighted = false;
                        @Override
                        public void mouseClicked(MouseEvent e) {
                            // this makes the square stayed highlighted when you click it and unhighlight when you click it again.
                            // then if the square has a piece on it, it will cal the availableMoves function to high light the
                            // available moves for that piece, then when the piece is clicked again to unhighlight it removes the
                            // available moves squares highlighting
                            if(isHighlighted){
                                setBorder(BorderFactory.createLineBorder(Color.DARK_GRAY, 3));
                                gb.removeAvailableMoves(moves, "DarkGrey");
                            }
                            else if(!isHighlighted && (hasPiece.equals("Black") || hasPiece.equals("Red"))){
                                setBorder(BorderFactory.createLineBorder(Color.MAGENTA, 3));
                                if(hasPiece.equals("Black")) {
                                    availableMoves(val, "Black", kingStatus);
                                    gb.showAvailableMoves(moves);
                                }
                                else if(hasPiece.equals("Red")){
                                    availableMoves(val, "Red", kingStatus);
                                    gb.showAvailableMoves(moves);

                                }
                            }
                            isHighlighted = !isHighlighted;
                        }

                        @Override
                        public void mousePressed(MouseEvent e) { }

                        @Override
                        public void mouseReleased(MouseEvent e) { }

                        @Override
                        public void mouseEntered(MouseEvent e) { }

                        @Override
                        public void mouseExited(MouseEvent e) { }
                    });

                }
                else {
                    this.setBackground(Color.BLACK);
                }
            }
        }
    }
