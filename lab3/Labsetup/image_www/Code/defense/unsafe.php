<?php
// Function to create a sql connection.
function getDB() {
  $dbhost="10.9.0.6";
  $dbuser="seed";
  $dbpass="dees";
  $dbname="sqllab_users";

  // Create a DB connection
  $conn = new mysqli($dbhost, $dbuser, $dbpass, $dbname);
  if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error . "\n");
  }
  return $conn;
}

$input_uname = $_GET['username'];
$input_pwd = $_GET['Password'];
$hashed_pwd = sha1($input_pwd);

// create a connection
$conn = getDB();

// do the query
//$result = $conn->query("SELECT id, name, eid, salary, ssn
//                        FROM credential
//                        WHERE name= '$input_uname' and Password= '$hashed_pwd'");
// do prepare
if ($stmt = $conn->prepare("SELECT id,name,eid,salary,ssn FROM credential WHERE name=? AND Password=?")) {
  // bind parameters
  $stmt->bind_param("ss", $input_uname, $hashed_pwd);
  // execute
  $stmt->execute();
  $stmt->bind_result($id, $name, $eid, $salary, $ssn);
  $stmt->fetch();
  $stmt->close();
}

// close the sql connection
$conn->close();
?>
