<?php include("connect.php"); ?>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Data Submit</title>
<link href='https://fonts.googleapis.com/css?family=Comfortaa' rel='stylesheet'>
<style type="text/css">

body {
font-family: 'Comfortaa';font-size: 22px;
background-color: #38095e
}

</style>
</head>
<body>
  <div align="center">
    <p><BR>
      <?php 
    
    $username = $_POST['username'];
    $password = $_POST['password'];
    $age = $_POST['age'];
    $sex = $_POST['sex'];
    $email = $_POST['email'];
    $card = $_POST['card'];
    $exp_date = $_POST['exp_date']; 
	$cvv = 	$_POST['cvv']; 
  
	
	// list($y, $m, $d) = explode("-", $exp_date);
	// if(!checkdate($m, $d, $y)){
		// $exp_date = 'NULL';
	// }
	
	$flag1 = 0;
	$flag2 = 0;
	list($y, $m, $d) = explode("-", $date);
	if(!is_string($m)){
		$flag1 = 1;
	}
	
	if (!is_int($age)){
		$flag2=1;
	}
	
	//test output for understanding
	// echo("<table><tr><th>username</th><th>passwrord</th><th>age</th><th>sex</th><th>email</th><th>card</th><th>date</th><th>cvv</th></tr>");
	// echo(" <tr> <td> $username , strlen($username) </td><td> $password	</td><td> $age , strlen($age) </td><td> $sex </td><td> $email  </td><td> $card ,strlen($card)</td><td> $exp_date ,strlen($exp_date) </td><td> $cvv ,strlen($cvv)</td> </tr> </table>");

	// echo '<p> this is strlen age</p>';
	// echo strlen($age);
	
	
	//MANAGING AGE(INT) EXP_DATE(DATE) ..GIVING VALUE NULL IF THE FORM IS EMPTY  
	if ($flag1==1 and $flag2==1){	
	$query = "INSERT INTO USER (Username, Password, Age, Sex, Email, Card_num, Exp_date, CVV) VALUES 
	('$username','$password',NULL,'$sex','$email','$card',NULL,'$cvv')";	
	}elseif ($flag1==1){
    $query = "INSERT INTO USER (Username, Password, Age, Sex, Email, Card_num, Exp_date, CVV) VALUES 
	('$username','$password',NULL,'$sex','$email','$card',NULL,'$cvv')";
	}elseif ($flag2==1){
	$query = "INSERT INTO USER (Username, Password, Age, Sex, Email, Card_num, Exp_date, CVV) VALUES 
	('$username','$password',NULL,'$sex','$email','$card','$exp_date','$cvv')";
	}else {
	$query = "INSERT INTO USER (Username, Password, Age, Sex, Email, Card_num, Exp_date, CVV) VALUES 
	('$username','$password','$age','$sex','$email','$card','$exp_date','$cvv')";
	}
  
	// echo $query;

    //execute query 
    $queryexe = mysqli_query($con, $query);
    
    if (!$queryexe){
        echo("Error description: " . mysqli_error($con));
    }else{
        echo("Success 1/2");
    }

	//updating all remaining empty strings into NULL values   UPDATE USER SET Exp_date=NULL where Exp_date='';
	$query2 = "UPDATE USER SET Sex = NULLIF(Sex, ''), Card_num = NULLIF(Card_num, ''), CVV = NULLIF(CVV, '') where Username='$username';";

	    $queryexe = mysqli_query($con, $query2);
    
    if (!$queryexe){
        echo("Error2 description: " . mysqli_error($con));
    }else{
        echo("<br>Success 2/2!");
    }

?>
      </p>
    <a href="GAMEusers.php">Πίσω</a><BR>
</div>
</body>
</html>