import javax.swing.*;
import java.awt.*;
import java.awt.AlphaComposite;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

public class FlameCheckersGUI {
    //create Frame of GUI
    private JFrame frame = new JFrame();

    //create Panel to hold title and title lable
    private JPanel titlePanel = new JPanel();
    private JLabel titleText = new JLabel( "Flame Checkers");

    //Create Image Panel and Label to hold image
    private JPanel imagePanel = new JPanel();
    private JLabel image =
            new JLabel(new ImageIcon("./charizardpic.png"));

    //create button panel and all buttons: exit, options, start
    private JPanel buttonPanel = new JPanel();
    private JButton exitButton = new JButton("Exit");
    private JButton optionsButton = new JButton("Options Menu");
    private JButton startButton = new JButton("Start Game");

    OptionsChosen optionsChosen = new OptionsChosen();

    public FlameCheckersGUI() {
        //initial frame implementation
        frame.setSize(900,700);
        frame.setTitle("Flame Checkers");
        frame.getContentPane().setBackground(Color.RED);
        frame.setResizable(false);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLocationRelativeTo(null);

        //add title
        titlePanel.setBackground(Color.RED);
        titlePanel.add(titleText);

        //add image
        imagePanel.setBackground(Color.RED);

        //add each button and set command for corresponding button
        buttonPanel.setBackground(Color.RED);
        //even to be exit program
        exitButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.exit(0);
            }
        });
        buttonPanel.add(exitButton, BorderLayout.LINE_START);
        //event to be open option windows
        optionsButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                OptionWindow optionWindow = new OptionWindow(optionsChosen.getGameMode(), optionsChosen.getPvPMode(),
                        optionsChosen.getAIDifficulty(), optionsChosen.getNightMode());
                optionsChosen.setGameMode(optionWindow.getGameMode());
                optionsChosen.setPvPMode(optionWindow.getPvPMode());
                optionsChosen.setAIDifficulty(optionWindow.getAIDifiiculty());
                optionsChosen.setNightMode(optionWindow.getNightMode());
                if(optionsChosen.getNightMode() == true){
                    frame.getContentPane().setBackground(Color.GRAY);
                    titlePanel.setBackground(Color.GRAY);
                    imagePanel.setBackground(Color.GRAY);
                    buttonPanel.setBackground(Color.GRAY);
                }
                else{
                    frame.getContentPane().setBackground(Color.RED);
                    titlePanel.setBackground(Color.RED);
                    imagePanel.setBackground(Color.RED);
                    buttonPanel.setBackground(Color.RED);
                }
            }
        });
        buttonPanel.add(optionsButton,BorderLayout.CENTER);
        //event to be open board window and instantiate instance of a game
        startButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                GameBoard gameBoard = new GameBoard(optionsChosen);
            }
        });
        buttonPanel.add(startButton ,BorderLayout.LINE_END);

        //add panels set visibility true
        frame.add(image, BorderLayout.CENTER);
        frame.add(titlePanel, BorderLayout.NORTH);
        frame.add(buttonPanel, BorderLayout.SOUTH);
        frame.setVisible(true);
    }


    }
