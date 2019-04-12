import javax.swing.*;
import java.awt.*;
import java.awt.Graphics;
import javax.swing.JFrame;
import javax.swing.JPanel;
import java.awt.Color;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

public class GameBoard extends JPanel {
    public JFrame boardFrame = new JFrame();
    JPanel border = new JPanel();

    public GameData gameData = new GameData();

    // 2 dimensional array which will work as a map for the squares to fill up the board
    public static CheckerSquare[][] square = new CheckerSquare[8][8];;


    public GameBoard (OptionsChosen options) {


        // sets the size for the checkerboard
        this.setSize(700, 700);
        boardFrame.setLayout(new GridLayout(8,9));

        boolean mode = options.getNightMode();
        CheckerSquare.NIGHT = mode;
        //This creates the checkerSquares using the appropriate coordinate, height, length, and value.
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {

                    square[i][j] = new CheckerSquare(j * CheckerSquare.WIDTH, i * CheckerSquare.HEIGHT, CheckerSquare.NIGHT);
                    square[i][j].color(this);
                    boardFrame.add(square[i][j]);
            }
        }

        // sets it visible
        boardFrame.setResizable(false);
        boardFrame.setSize(700, 700);
        boardFrame.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
        boardFrame.setLocationRelativeTo(null);
        boardFrame.setVisible(true);
        repaint();
    }

    // Highlights squares that are available for a piece to be moved to
    public void showAvailableMoves(int[] moves){
        int i, j, k;
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                for (k = 0; k < 4; k++){
                    if(square[i][j].val == moves[k] && square[i][j].hasPiece.equals("None")){
                        square[i][j].setBorder(BorderFactory.createLineBorder(Color.MAGENTA, 3));
                    }
                }
            }
        }
    }

    // Removes the highlighted squares that are available moves
    public void removeAvailableMoves(int[] moves, String color){
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                for (int k = 0; k < 4; k++){
                    if(square[i][j].val == moves[k]){
                        if(color.equals("Red"))
                            square[i][j].setBorder(BorderFactory.createLineBorder(Color.RED, 3));
                        else if(color.equals("DarkGrey"))
                            square[i][j].setBorder(BorderFactory.createLineBorder(Color.DARK_GRAY, 3));
                    }
                }
            }
        }
    }

    //This prints the actual squares onto the board in the appropriate locations
    @Override
    public void paintComponent(Graphics g) {
        for (int i = 0; i < square.length; i++) {
            for (int j = 0; j < square[i].length; j++) {
                //call the draw method on each tile.
                square[i][j].print(g);
            }
        }
    }
}
