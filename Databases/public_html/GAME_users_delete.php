<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Delete a user</title>
<link href='https://fonts.googleapis.com/css?family=Comfortaa' rel='stylesheet'>

<style type="text/css">
body {
font-family: 'Comfortaa';font-size: 22px;
background-color: #38095e
}

.style1{
font-size:40px;
}

</style>
<script language="JavaScript" src="files/gen_validatorv31.js" type="text/javascript"></script>
</head>

<body>
<div class="delete_user" align="center">
<center>
  <table width="400" border="0">
    <tr>
       <td><div align="center" class="style1">Delete User</div></td>
    </tr>
  </table>
  <p align="center">&nbsp;</p>
</center>
    <form method="POST" action="GAME_users_submit_delete.php"  name="myform2">
      <center>
      <table>
        <tr>
        <td>
      <div align="center"><strong>Username:</strong> 
        <select class="select" name="username">
        <option value="">Choose...</option>
		<?php
		include('connect.php');
		
		
		 $query = "SELECT Username from USER"; 
     
		//execute query 
		$result = mysqli_query($con, $query); 

		if (!$result){
			echo("Error description: " . mysqli_error($con));
		}
		
		//query database 
		while($rows = mysqli_fetch_array($result)){
			$username = $rows['Username'];

		//format results
			echo ("<option value=\"$username\">$username</option>");   
		
		}  

		?>
        <input type="submit" value="delete" /></div>
        </td>
        </tr>
        </table> 
      </center>
</form>
<br>
<a href="GAMEusers.php">back</a>
</div>


<script language="JavaScript" type="text/javascript">
//You should create the validator only after the definition of the HTML form
  var frmvalidator  = new Validator("myform2");
  frmvalidator.EnableMsgsTogether();
  frmvalidator.addValidation("username","req","Fill the field \"Username\" !");
  frmvalidator.addValidation("username","dontselect=0","Fill the field \"Username\"!");
      </script>
</body>
</html>