public class OptionsChosen {
    private String GameMode = "PvP";
    private String PvPMode = "Normal";
    private String AIDifficulty = "Easy";
    private boolean NightMode = false;

    public OptionsChosen() {
        GameMode = "PvP";
        PvPMode = "Normal";
        AIDifficulty = "Easy";
        NightMode = false;
    }

    public String getGameMode () { return GameMode; }
    public String getPvPMode() { return PvPMode; }
    public String getAIDifficulty () { return AIDifficulty; }
    public boolean getNightMode () { return NightMode; }

    public void setGameMode(String s) { GameMode = s; }
    public void setPvPMode(String s) { PvPMode = s; }
    public void setAIDifficulty(String s) { AIDifficulty = s; }
    public void setNightMode(boolean b) { NightMode = b; }
}
