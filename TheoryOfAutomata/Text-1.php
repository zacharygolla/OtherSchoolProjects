<?php

#Open Page and Retrieve all employee URLS
$facultyPage = file_get_contents('https://cs.txstate.edu/accounts/faculty/');
$parseUrls = '';
preg_match_all('/<a href="(\/accounts\/profiles\/.*?)\/">[a-zA-Z .,-]*<\/a>/', $facultyPage, $parseUrls);
$individualUrls = $parseUrls[1];

$count = count($individualUrls);
for($x = 0; $x < $count; $x++) {
    $const_string = 'https://cs.txstate.edu';
    $indivUrl = $const_string.$individualUrls[$x];
    $individualPage = file_get_contents($indivUrl);
    
    //open output file to put out to
    //$outputFile = fopen('\Users\Zac\TestOutput\https://cs.txstate.edu/'.$individualUrls[$x].'.txt', 'w');

    #Get Professor's Name
    $name = 'Name: ';
    preg_match('/<title>([A-Za-z -.]*)<\/title>/', $individualPage, $nameParse);
    $testName = $nameParse[1];
    $nameParts = explode(" ", $testName);
    
    $startConc = false;
    $countNameParts = count($nameParts);
    for($j = 0; $j < $countNameParts; $j++) {
        if($startConc) {
            $name = $name.$nameParts[$j]." ";
        }
        if($nameParts[$j] == "Dr." || $nameParts[$j] == "Mr." || $nameParts[$j] == "Ms." || $nameParts[$j] == "Mrs.") {
            $startConc = true;
        }
    }
    //fwrite($outputFile, $name."\n");
    echo $name, "\n";
    
    $edPrompt = 'Education: ' ;
    preg_match('/<h3 class="panel-title">Education<\/h3>[ \n]*<\/div>[ \n]*[a-z<="-> ]*([a-zA-Z ,;&]*)<\/p>/', $individualPage, $edParse);
    if(!$edParse)
        $ed = $edPrompt."No education listed";
    else {
        $ed = $edPrompt.$edParse[1];
    }
    //fwrite($outputFile, $ed."\n");
    
    echo $ed, "\n";

    $resPrompt = 'Research Interest: ';
    preg_match('/<h3 class="panel-title">Research Interests<\/h3>[ \n]*<\/div>[ \n]*[a-z<="-> ]*([a-zA-Z ,;&]*)<\/p>/', $individualPage, $resParse);
    if(!$resParse)
        $res = $resPrompt."No research interests listed";
    else {
        $res = $resParse[1];
    }
    $res = $resPrompt.$res;
    //fwrite($outputFile, $res."\n");
    echo $res, "\n";
    

    $emailPrompt = 'Email: ';

    preg_match('/<img style="padding-left: 2px;" class="email-image" src="\/cms\/email_image\/[?]user=([a-zA-z0-9]*)&domain=txstate.edu/', $individualPage, $emailParse);
    if(!$emailParse)
        $email = $emailPrompt."No research interests listed";
    else {
        $email = $emailParse[1];
    }
    //fwrite($outputFile, $email.'@txstate.edu'."\n");
    echo $emailPrompt.$email.'@txstate.edu', "\n";

    $webPrompt = "Webpage: ";  
    $webPage = $webPrompt.$indivUrl;
    //fwrite($outputFile, $webPage);
    echo $webPage, "\n";
    
    echo "\n";
}

echo "Run Complete";


?>