//homepage.h
//Mikaela Diaz

// Display the HTML web page using CSS
// Using flash memory to store non-changing parts of the web page
String mainPage = (F(R"=====(
<!DOCTYPE html>
<html lang = "eng">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale = 1">
    <link rel="icon" href="data:,">
    <style>
        html {
            font-family: Amasis MT Pro Medium;
            display: inline-block;
            margin: 0px auto;
            text-align: center;
            color: black;
            background-color: rgb(188, 248, 203);
        }
        img {
            border: 5px solid black;
            padding: 3px;
            margin: 5px;
        }
        p {
            font-size: 180%;
            font-family: Arial, Helvetica, sans-serif;
        }
        .button {
            background-color: #ec0909;
            border: 5px solid black;
            padding: 3px;
            color: white;
            padding: 22px 60px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 4px 2px;
            cursor: pointer;
        }
        table {
            font-family: arial, sans-serif;
            border-collapse: collapse;
            width: 70%;
        }
        table,
        td,
        th {
            border: 2px solid #F20019;
            text-align: center;
            padding: 15px;
        }
        table th {
            background-color: #fad3a0;
        }
        tr:nth-child(even) {
            background-color: #faf287;
        }

        table.center {
            margin-left: auto;
            margin-right: auto;
        }
        .flex-container {
            display: flex;
            flex-direction: row;
            align-items: center;
            flex-direction: column;
            background-color: aqua;
        }
        .flex-container>div {
            background-color: #f1f1f1;
            width: 800px;
            margin: 5px;
            padding: 5px;
            justify-content: center;
            text-align: center;
            line-height: 30px;
        }
        @media (max-width: 800px) {
            .flex-container{
                flex-direction: column;
            }
        }
        </style>
        <title>Health-Tracking Webpage</title>
</head>
<body>
    <div id='main'>
        <h1 style="font-size: 250%;">Health-Tracking Website with ESP32 Server</h1>
        <p><b><i>Welcome to your Homepage</i></b></p>
        <img src="https://live.staticflickr.com/65535/52696081361_533bc99af1_z.jpg" width="600" height="340"
             alt="Health Tracking Smartwatch LCD Screen Image">
        <script async src="//embedr.flickr.com/assets/client-code.js" charset="utf-8">
        </script>
        <p style="font-size: 120%;">Here are some readings of your Health-Tracking Smartwatch.</p>
    </div>
        <h2>Send Location Button</h2>
    <div class="flex-keypad">
        <button class="button" onclick="fetch('/sentKeyPressToWebServer')">SMS</button>
        <a onclick="fetch('/sentKeyPressToWebServer?button=sendSMS_GPS')"></a> 
    </div>
</body>
</html>
)====="));
