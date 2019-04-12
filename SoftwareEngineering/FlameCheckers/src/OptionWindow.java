import javax.swing.*;
import java.awt.*;
import java.awt.AlphaComposite;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

public class OptionWindow extends JDialog {

    //create Panel to hold title and title lable
    private JPanel TwoPlayerPanel = new JPanel();
    private JPanel confirmationPanel = new JPanel();
    private JPanel AIPanel = new JPanel();
    private JPanel modePanel = new JPanel();

    private ButtonGroup PvPgroup = new ButtonGroup();
    private ButtonGroup modeGroup = new ButtonGroup();
    private ButtonGroup AIGroup = new ButtonGroup();

    private JRadioButton normalMode = new JRadioButton("Normal Mode");
    private JRadioButton timedMode = new JRadioButton("Timed Mode");
    private JRadioButton PvPMode = new JRadioButton("Player vs. Player");
    private JRadioButton AIMode = new JRadioButton("Player vs. AI");
    private JRadioButton easyMode = new JRadioButton("Easy");
    private JRadioButton mediumMode = new JRadioButton("Medium");
    private JRadioButton hardMode = new JRadioButton("Hard");
    private JCheckBox nightMode = new JCheckBox("Night Mode");
    private JButton confirmButton = new JButton("Confirm");
    private JButton cancelButton = new JButton("Cancel");

    private String GameMode;
    private String PvP_Mode ;
    private String AIDifficulty;
    private boolean NightMode;

    // Getters for the options
    public String getGameMode(){
        return GameMode;
    }
    public String getPvPMode(){
        return PvP_Mode;
    }
    public String getAIDifiiculty(){
        return AIDifficulty;
    }
    public boolean getNightMode(){
        return NightMode;
    }

    public OptionWindow(String gm, String ppm, String aid, boolean nm) {
        //Makes the window remember its previous states
        GameMode = gm;
        PvP_Mode = ppm;
        AIDifficulty = aid;
        NightMode = nm;

        //initial frame implementation
        setModal(true);
        setSize(500,200);
        setTitle("Options");

        // Changes the background depending on if night mode is selected
        if(NightMode == true){
            getContentPane().setBackground(Color.GRAY);
        }
        else{
            getContentPane().setBackground(Color.ORANGE);
        }
        setResizable(false);
        setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
        setLocationRelativeTo(null);




        //Sets up the panel that allows the selection of the game modes
        if(NightMode == true){
            modePanel.setBackground(Color.GRAY);
        }
        else{
            modePanel.setBackground(Color.ORANGE);
        }
        modePanel.setBorder(BorderFactory.createTitledBorder("Game Mode"));
        PvPMode.setActionCommand("PvP");
        AIMode.setActionCommand("PvAI");
        modeGroup.add(PvPMode);
        modeGroup.add(AIMode);
        if(GameMode.equals("PvP")){
            PvPMode.setSelected(true);
        }
        else{
            AIMode.setSelected(true);
        }
        modePanel.add(PvPMode);
        modePanel.add(AIMode);




        //Sets up the panel that selects PvP game settings
        if(NightMode == true){
            TwoPlayerPanel.setBackground(Color.GRAY);
        }
        else{
            TwoPlayerPanel.setBackground(Color.ORANGE);
        }
        TwoPlayerPanel.setBorder(BorderFactory.createTitledBorder("Player vs. Player"));
        normalMode.setActionCommand("Normal");
        timedMode.setActionCommand("Timed");
        PvPgroup.add(normalMode);
        PvPgroup.add(timedMode);
        if(PvP_Mode.equals("Normal")){
            normalMode.setSelected(true);
        }
        else{
            timedMode.setSelected(true);
        }
        TwoPlayerPanel.add(normalMode);
        TwoPlayerPanel.add(timedMode);




        //Sets up the panel that selects PvAI game settings
        if(NightMode == true){
            AIPanel.setBackground(Color.GRAY);
        }
        else{
            AIPanel.setBackground(Color.ORANGE);
        }
        AIPanel.setBorder(BorderFactory.createTitledBorder("Player vs. AI"));
        easyMode.setActionCommand("Easy");
        mediumMode.setActionCommand("Medium");
        hardMode.setActionCommand("Hard");
        AIGroup.add(easyMode);
        AIGroup.add(mediumMode);
        AIGroup.add(hardMode);
        if(AIDifficulty.equals("Easy")){
            easyMode.setSelected(true);
        }
        else if(AIDifficulty.equals("Medium")){
            mediumMode.setSelected(true);
        }
        else{
            hardMode.setSelected(true);
        }
        AIPanel.add(easyMode);
        AIPanel.add(mediumMode);
        AIPanel.add(hardMode);



        //Sets up the panel that allows for night mode and conformation/cancelation of game settings
        if(NightMode == true){
            confirmationPanel.setBackground(Color.GRAY);
        }
        else{
            confirmationPanel.setBackground(Color.ORANGE);
        }

        // Creates confirmation button that saves options that are chosen
        confirmButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if(nightMode.isSelected()){
                    NightMode = true;
                }
                else{
                    NightMode = false;
                }

                if(modeGroup.getSelection().getActionCommand().equals("PvP")){

                    GameMode = "PvP";

                    if(PvPgroup.getSelection().getActionCommand().equals("Normal")){
                        PvP_Mode =  "Normal";
                    }
                    else{
                        PvP_Mode = "Timed";
                    }
                }
                else{

                    GameMode = "PvAI";

                    if(AIGroup.getSelection().getActionCommand().equals("Easy")){
                        AIDifficulty = "Easy";
                    }
                    else if(AIGroup.getSelection().getActionCommand().equals("Medium")){
                        AIDifficulty = "Medium";
                    }
                    else{
                        AIDifficulty = "Hard";
                    }
                }
                dispose();
            }
        });
        confirmationPanel.add(confirmButton, BorderLayout.CENTER);

        // Adds a cancel button that doesnt save selected options
        cancelButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                dispose();
            }
        });
        confirmationPanel.add(cancelButton, BorderLayout.LINE_START);


        // Changes color of back grounds if night mode is on
        if(NightMode == true){
            nightMode.setSelected(true);
        }
        else{
            nightMode.setSelected(false);
        }
        nightMode.addItemListener(new ItemListener() {
            @Override
            public void itemStateChanged(ItemEvent e) {
                if(e.getStateChange() == ItemEvent.SELECTED) {
                    confirmationPanel.setBackground(Color.GRAY);
                    AIPanel.setBackground(Color.GRAY);
                    TwoPlayerPanel.setBackground(Color.GRAY);
                    modePanel.setBackground(Color.GRAY);
                    getContentPane().setBackground(Color.GRAY);
                }else {
                    confirmationPanel.setBackground(Color.ORANGE);
                    AIPanel.setBackground(Color.ORANGE);
                    TwoPlayerPanel.setBackground(Color.ORANGE);
                    modePanel.setBackground(Color.ORANGE);
                    getContentPane().setBackground(Color.ORANGE);
                }
            }
        });
        confirmationPanel.add(nightMode, BorderLayout.LINE_END);

        //add panels set visibility true
        add(modePanel, BorderLayout.NORTH);
        add(TwoPlayerPanel, BorderLayout.WEST);
        add(AIPanel, BorderLayout.EAST);
        add(confirmationPanel, BorderLayout.SOUTH);
        setVisible(true);
    }
}
