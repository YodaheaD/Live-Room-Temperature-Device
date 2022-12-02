<!DOCTYPE html>
<html>
	
<head>
<meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
	<link rel="stylesheet"  href="index.css"/>
	<title>
		Database Data Web-sheet
	</title>
</head>

<body style="text-align:center;">
	
	<h1 style="color:green;">
		Getting Database data
	</h1>


	<?php
	
	
	/// --- Displaying the Max function --- ///

	function GetMax($conn,$Col_Name){
		$result = mysqli_query($conn, "SELECT id, MAX($Col_Name) as 'max' FROM sensor");
		$row = mysqli_fetch_array($result);
		$str1=$row['max'];


		$result2 = mysqli_query($conn, "SELECT id, MIN($Col_Name) as 'min' FROM sensor");
		$row2 = mysqli_fetch_array($result2);


		echo '<table cellspacing="9" cellpadding="5"> 
			  <br><tr><br> Data 
			    <td>(ID Max)</td> 
				<td>Max</td> 
				<td>(ID Min)</td> 
				<td>Min</td> 
			  </tr>';
		echo '<tr> 
			<td>' . $row['id'] . '</td> 
			<td>' . $row['max']. '</td> 
			<td>' . $row2['id'] . '</td> 
			<td>' . $row2['min']. '</td>
			</tr>';

	}

	/// --- Displaying the Data function --- ///
	function DisplayTable($conn,$No_of_Rows){
		$sql = "SELECT id, value1, value2, reading_time FROM sensor ORDER BY id DESC LIMIT $No_of_Rows";
				
		//-- Create table --//
		echo '<table cellspacing="9" cellpadding="5"> 
			  <tr><br>Table Data 
				<td>ID</td> 
				<td>Temperature</td> 
				<td>Humidity</td>
				<td>Timestamp</td> 
			  </tr>';
		 
		if ($result = $conn->query($sql)) {
			while ($row = $result->fetch_assoc()) {
				$row_id = $row["id"];
				$row_value1 = $row["value1"];
				$row_value2 = $row["value2"]; 
				$row_reading_time = $row["reading_time"];

			//-- Put values into Data table --//
				echo '<tr> 
						<td>' . $row_id . '</td> 
						<td>' . $row_value1 . '</td> 
						<td>' . $row_value2 . '</td>
						<td>' . $row_reading_time . '</td> 
					  </tr>';
			}
			$result->free();
		}
		$conn->close();


		
	}
		///// -- End of Function -- //////

	// Connection Details for the Database
	$servername = "localhost";
	$dbname = "db_arduino";
	$username = "Arduino";
	$password = "ArduinoGetStarted.com";
	
	///////////////////////////////////////
	
	
	$conn = new mysqli($servername, $username, $password, $dbname);
	// Check connection
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	} 
	///
	
	if(isset($_POST['button1'])) {
		echo " Button1 is selected";

		// Establish Number of Rows
		$No_of_Rows=5;
		DisplayTable($conn,$No_of_Rows);
	}
	if(isset($_POST['button2'])) {
		echo " Button2  is selected";

		// Establish Number of Rows
		$No_of_Rows=1;
		DisplayTable($conn,$No_of_Rows);
	}
	if(isset($_POST['button3'])) {
		echo " Button3 is selected";
		$Col_Name = "value1";
		GetMax($conn,$Col_Name);
	}
	if(isset($_POST['button4'])) {
		echo " Button4 is selected";
		$Col_Name = "value1";
	
		
	}
		?>
	
	<form method="post">
		<input type="submit" name="button1"
				value="Button1"/>
		
		<input type="submit" name="button2"
				value="Button2"/>

		<input type="submit" name="button3"
				value="Button3"/>
		
		<input type="submit" name="button4"
				value="Button4"/>
	</form>
</head>

</html>
