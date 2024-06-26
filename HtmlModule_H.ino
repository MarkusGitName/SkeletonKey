// Module1.cpp
#include "html.h"

//css 
       
String style() 
{
    return R"(
        <style>
            body {
                font-family: 'Arial', sans-serif;
                background-color: #333;
                padding: 0;
                text-align: center;
                color: #fff;
            }

            table {
                max-width: 50%;
                margin:auto;
                border-collapse: collapse;
                background-color: #444;
                box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
                color: #fff;
            }
            .tab-content {
            }

            img {
                padding: 40px;
                max-width: 80%;
              
            }

            h1, h2, h3, a {
                color: #007bff;
            }

            th, td {
                padding: 15px;
                text-align: left;
                border-bottom: 1px solid #555;
            }

            th {
                background-color: #007bff;
                color: #fff;
            }

            button {
                background-color: #28a745;
                color: #fff;
                padding: 10px 20px;
                border: none;
                border-radius: 5px;
                cursor: pointer;
                font-size: 16px;
            }

            button:hover {
                background-color: #218838;
            }

            form {
                max-width: 100%;
                background-color: #555;
                padding: 20px;
                border-radius: 8px;
                box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
                color: #fff;
            }

            label {
                display: block;
                margin-bottom: 8px;
                margin-top: 10px;
            }

            input, select {
                width: 70%;
                padding: 10px;
                margin-bottom: 16px;
                box-sizing: border-box;
                border: 1px solid #777;
                border-radius: 4px;
                font-size: 16px;
                background-color: #333;
                color: #fff;
            }
            .tabs {
              min-width: 80%;
              margin:auto
            }
            .tab {
              min-width: 20%;
              margin:auto
            }
            .container {
                display: grid;
                grid-template-columns: 50% 50%;
                width: 100%;
                min-height: 100vh; /* Ensure the container spans the full height of the viewport */
            }

            .left-column {
              margin-top: 0
            }

            .right-column {
                margin-top: 0;
                text-align:center;
            }
            
          .dropdown {
              position: relative;
              display: inline-block;
              margin-top: 20px;
              margin-right: 20px;
          }

          .dropdown-content {
              display: none;
              position: absolute;
              top: 100%;
              right: 0;
              background-color: #f9f9f9;
              min-width: 160px;
              box-shadow: 0 2px 5px rgba(0,0,0,0.2);
              z-index: 1;
          }

          .dropdown-content a {
              display: block;
              padding: 10px 15px;
              text-decoration: none;
              color: #333;
          }

          .dropdown:hover .dropdown-content {
              display: block;
          }
              
          @media screen and (max-width: 650px) {
              body {
                  max-width: 100%;
              }
              img {
                  max-width: 100%;
              }
              tabs {
                max-width: 80%
              }
              .container {
                  grid-template-columns: 100%; /* Switch to single column layout */
              }

              .left-column {
                  order: 2; /* Move left column to the bottom */
              }
          }
        .remote {
            width: 250px;
            background: rgb(64,64,72);
		      	background: radial-gradient(circle, rgba(64,64,72,1) 0%, rgba(86,84,83,0.9271358201483718) 50%, rgba(5,41,48,1) 85%);
            padding-top: 25px;
            padding:auto;
            margin:auto;
            border-radius: 15px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.5), /* Outer shadow */
                        0 0 20px rgba(0, 0, 0, 0.5), /* Outer shadow */
                        0 0 30px rgba(0, 0, 0, 0.5), /* Outer shadow */
                        inset 0 0 10px rgba(0, 0, 0, 0.5), /* Inner shadow */
                        inset 0 0 20px rgba(0, 0, 0, 0.5); /* Inner shadow */
            text-align: center; /* Align the buttons in the center */
            position: relative; /* To position the LED */
        }

        .button {
            display: inline-block;
            width: 85px; /* Width of each button */
            height: 85px; /* Height of each button */
            margin: 15px;
            padding-top: 10%;
            border-radius: 15px; /* Make the buttons squared */
            cursor: pointer;
            background-color: #ddd; /* Default button color */
            border: 1px solid #bbb; /* Button border */
            box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.5), /* Button shadow */
                        inset -2px -2px 5px rgba(255, 255, 255, 0.1); /* Button highlight */
            transition: background-color 0.3s ease, transform 0.2s ease;
        }

        .button:hover {
            background-color: #ccc; /* Darker color on hover */
            transform: translate(0, 2px); /* Move down slightly on hover */
            box-shadow: 4px 4px 10px rgba(0, 0, 0, 0.7), /* Button shadow */
                        inset -4px -4px 10px rgba(255, 255, 255, 0.1); /* Button highlight */
        }

        .led {
            width: 20px;
            height: 20px;
            background-color: #ff0000; /* Initial LED color */
            border-radius: 50%;
            position: absolute;
            top: -5px; /* Adjust position above the buttons */
            left: 50%; /* Center the LED horizontally */
            transform: translateX(-50%); /* Center the LED horizontally */
            display: none; /* Initially hidden */
        }
        </style>
    )";
}

//HTML stuff ***************
String UserPage()
{
  return ""
    "<h1>Users</h1>"
    "<button onclick='openTab(\"userForm\")'>Create New User</button>"
    +usersList();
}

String KeyPage(String url, String msg, String domainName)
{
  return ""
    "<h1>Keys</h1>"
    "<button onclick='openTab(\"keyPage\")'>Create New Virual Key</button>"
    +keyList(url, msg, domainName);
}

String usersList() 
{
  String result = "<table> ";

    result+= 
    "<tr>"
      "<th>Use Name</th>"
      "<th>Category</th>"
      "<th>Options</th>"
      "</tr>";
  for (int i = 0; i < numUsers; i++) {
    result+= 
    "<tr>"
      "<td>" + String(users[i].username) + "</td>"
      "<td>" + String(users[i].category) + "</td>"
      "<td> <button onclick=\"removePortalUser("+i+")\">Delete user</button></td>"
    "</tr>";
  }

  result += "</tr></table>"
      "<script>"
      "function removePortalUser(user) {"
      "  fetch('/removeUser?user=' + user +'&'+ loginParams.toString())"
      "    .then(response => response.text())"
      "    .then(data => alert(data));"
      "}"
      "</script>";
  return result;
}

String keyList(String url, String msg, String domainName) 
{
  String result = "<table> ";

    result+=
     "<tr>"
        "<th>key code</th>"
        "<th>key number</th>"
        "<th>Options</th>"
      "</tr>";
  result+="</tr><tr>";
  for (int i = 0; i < numKeys; i++) {
    result+= 
      "<tr>"
        "<td> " + String(keys[i].code) + "</td>"
        "<td> " + String(keys[i].remoteNumber) + "</td>"
        "<td><button onclick=\"removeKey("+i+")\">Delete Key</button></td>"
        "<td><button onclick=\"shareKeyOnWhatsapp('"+ String(keys[i].code) +"')\">Share via Whatsapp</button></td>"
      "</tr>";
  }

  result += ""
    "<textarea style='display:none' id='message' rows='10' cols='50'>"+msg+"</textarea>"
    "<p>Domain Name: <a id='url' href='http://"+domainName+".sk?virtualKeyCode=' ></a></p>"
    "<p>URL: <a id='url' href='"+url+"' ></a></p>";

  result += "</table>"
      "<script>"
      "function shareKeyOnWhatsapp(code) {"
              "var Message = document.getElementById('message').value;"//.replace(/\n/g, '%0A');"
              "var KeyLink = document.getElementById('url').href;"
              "var Url = document.getElementById('url').href;"
              "var whatsappUrl = 'https://wa.me/?text=' + encodeURIComponent(Message +'Key Code: ' + code + '. Key Link: ' + KeyLink + code+'. If the key link does not work, you can reach the portal by copying and pasting the following into your browser:'+ url);"

              // Open the WhatsApp share window
              "window.open(whatsappUrl, '_blank');"

      "}"
      "function removeKey(key) {"
      "  fetch('/removeVirtualKey?key=' + key +'&'+ loginParams.toString())"
      "    .then(response => response.text())"
      "    .then(data => alert(data));"
      "}"
      "</script>";
  return result;
}

String tabControl()
{
   return "<h1>Controls</h1>"
                   "<h3>Lights</h3>"
                    "<button onclick=\"toggleLight(1)\"> Light </button>"
                    
                  "<div class='remote'>"
                      "<div class='led' id='led'></div>"
                      "<!-- Set initial button colors -->"
                      "<div class='button' style='background: rgb(7,137,11);background: radial-gradient(circle, rgba(7,137,11,1) 80%, rgba(44,10,17,1) 100%);' onclick='signalRemote(1)'></div>"
                      "<div class='button' style='background: rgb(137,7,107);background: radial-gradient(circle, rgba(137,7,107,1) 80%, rgba(44,10,17,1) 100%);' onclick='signalRemote(2)'></div>"
                      "<div class='button' style='background: rgb(7,68,137);background: radial-gradient(circle, rgba(7,68,137,1) 80%, rgba(44,10,17,1) 100%);' onclick='signalRemote(3)'></div>"
                      "<div class='button' style='background: rgb(137,103,7);background: radial-gradient(circle, rgba(137,103,7,1) 80%, rgba(44,10,17,1) 100%);' onclick='signalRemote(4)'></div>"
                  "</div>"

                   "<script>"
                    "var isStreaming = false;"
                    "function toggleLight(light) {"
                    "  fetch('/toggle?light=' + light +'&'+ loginParams.toString())"
                    "    .then(response => response.text())"
                    "    .then(data => alert(data));"
                    "}"
                    "function signalRemote(remote) {"
                    // Change LED color
                      "var led = document.getElementById('led');"
                      "var buttons = document.getElementsByClassName('button');"
                      
                      "switch (remote) {"
                          "case 1:"
                              "led.style.backgroundColor = '#2b8106';" // green
                              "break;"
                          "case 2:"
                              "led.style.backgroundColor = '#810677';" // purple
                              "break;"
                          "case 3:"
                              "led.style.backgroundColor = '#066181';" // Blue
                              "break;"
                          "case 4:"
                              "led.style.backgroundColor = '#896707';" // Yellow
                              "break;"
                      "}"

                      // Turn on the LED for a second
                      "led.style.display = 'block';"
                      "setTimeout(function() {"
                          "led.style.display = 'none';"
                      "}, 1000);" // LED shines for 1 second"


                  //  "  fetch('/signal?remote=' + remote +'&'+ loginParams.toString())"
                  //  "    .then(response => response.text())"
                  //  "    .then(data => alert(data));"
                    "fetch('/signal?remote=' + remote + '&' + loginParams.toString())"
                    ".then(response => {"
                        "if (!response.ok) {"
                            // Handle HTTP error status
                            "throw new Error('HTTP error! Status: ${response.status}');"
                        "}"
                        "return response.text();"
                   " })"
                    ".then(data => alert(data))"
                    ".catch(error => {"
                        // Handle any errors that occurred during the fetch or in the previous then block
                        "alert('Error: ' + error.message);"
                    "});"

                    "}"
                    
                    "window.onload = function() {"
                        // Set initial LED color
                        "var led = document.getElementById('led');"
                        "led.style.display = 'block';"
                        "led.style.backgroundColor = '#ff0000';" // Initial LED color
                        "setTimeout(function() {"
                            "led.style.display = 'none';"
                        "}, 1000);" // Hide LED after 1 second
                    "};"

                   "</script>";
}
     
String LandinPage(String url,String url2)
{
  return ""
  "<!DOCTYPE html>"
  "<html lang='en'>"
  "<head>"
      "<meta charset='UTF-8'>"
      "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
      "<title>Portal Landing Page</title>"
      +style()+
  "</head>"
  "<body>"
  "<a href='"+url+"' id='myredirect' target='_blank'>Redirect to guest portal.</a><br/>"

  
  "<script>"
    // JavaScript code to redirect the user
        "window.onload = function() {"
            "const loginParams = new URLSearchParams(window.location.search);"

            "const baseURL = '"+url2+"';"
            "const baseURL2 = '"+url+"';" // Example fallback URL

            // Use backticks for template literals
            "const newURL = `${baseURL}?${loginParams.toString()}`;"
            "const newURL2 = `${baseURL2}?${loginParams.toString()}`;"

           " document.getElementById('myredirect').addEventListener('click', function(event) {"
                "event.preventDefault();"
                "const primaryUrl = newURL;"
                "const fallbackUrl = newURL2;"

                "fetch(primaryUrl, { method: 'HEAD' })"
                    ".then(response => {"
                        "if (response.ok) {"
                           " window.location.href = primaryUrl;"
                        "} else {"
                            "throw new Error('Primary URL failed');"
                        "}"
                    "})"
                   " .catch(error => {"
                        "window.location.href = fallbackUrl;"
                    "});"
            "});"
        "};"


  "</script>";
  "</body>"
  "</html>";
}

// guestmode pages
String GuestPage()
{
  return ""
  "<!DOCTYPE html>"
  "<html lang='en'>"
  "<head>"
      "<meta charset='UTF-8'>"
      "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
      "<title>Login Page with Tabs</title>"
      +style()+
  "</head>"
  "<body >"
  +tabControl()+
  
  "<script>"
      "const loginParams = new URLSearchParams(window.location.search);"
  "</script>";
  "</body>"
  "</html>";
}

String newUserForm()
{
  return ""
    "<h1>New User</h1>"
    "<form id='newUserForm'>"
        "<label for='newUser'>User Name:</label>"
        "<input type='text' id='newUser' name='newUser' value='""' required><br/>"

        "<label for='newUserPassword'>Password:</label>"
        "<input type='password' id='newUserPassword' name='newUserPassword' value='""' required><br/><br/>"

        "<!-- Category Dropdown -->"
        "<label for='newUsercategory'>Category:</label>"
          "<select id='newUsercategory' name='newUsercategory' required>"
              "<option value='superadmin'>superadmin</option>"
              "<option value='admin'>admin</option>"
              "<option value='stream'>stream</option>"
              "<option value='streamtenant'>streamtenant</option>"
              "<option value='resedant'>resedant</option>"
              "<option value='regular'>regular</option>"
              "<option value='guest'>guest</option>"
          "</select><br>"
        "<button type='button' onclick='saveUser()'>Save</button>"
    "</form><br/>"
    "<script>"
      "function saveUser() {"
          "var NewUser = document.getElementById('newUser').value;"
          "var NewUserPassword = document.getElementById('newUserPassword').value;"
          "var Category = document.getElementById('newUsercategory').value;"
          "var baseUrl = '/saveUser';"
          "var url = baseUrl + '?newUser=' + encodeURIComponent(NewUser) + '&newUserPassword=' + encodeURIComponent(NewUserPassword)+ '&category=' + encodeURIComponent(Category)+'&'+ loginParams.toString();"
          
                  //  "  fetch(url)"
                  //  "    .then(response => response.text())"
                  //  "    .then(data => alert(data));"
          "fetch(url)"
           " .then(response => {"
                "if (!response.ok) {"
                    // Handle HTTP error status
                    "throw new Error('HTTP error! Status: ${response.status}');"
               " }"
                "return response.text();"
           " })"
            ".then(data => alert(data))"
          "  .catch(error => {"
                // Handle any errors that occurred during the fetch or in the previous then block
               " alert('Error: ' + error.message);"
            "});"
         "location.reload();"
        "}"
    "</script>";
}

String userProfileForm()
{
  return ""
    "<h1>User Profile</h1>"
    "<form id='userProfileForm'>"
        "<label for='userName'>User Name:</label>"
        "<input type='text' id='userName' name='userName' value='' required><br/>"

        "<label for='oldPassword'>Password:</label>"
        "<input type='password' id='oldPassword' name='oldPassword' value='""' required><br/><br/>"
        "<label for='newPassword'>New Password:</label>"
        "<input type='password' id='newPassword' name='newPassword' value='""' required><br/><br/>"

        "<!-- Category Dropdown -->"
        "<label for='profilecategory'>Category:</label>"
          "<select id='profilecategory' name='profilecategory' disabled>"
              "<option value='superadmin'>superadmin</option>"
              "<option value='admin'>admin</option>"
              "<option value='stream'>stream</option>"
              "<option value='streamtenant'>streamtenant</option>"
              "<option value='resedant'>resedant</option>"
              "<option value='regular'>regular</option>"
              "<option value='guest'>guest</option>"
          "</select><br>"
        "<button type='button' onclick='saveProfile()'>Save</button>"
    "</form><br/>"
    "<script>"
    
      "function saveProfile() {"
            "var userName = document.getElementById('userName').value;"
            "var oldPassword = document.getElementById('oldPassword').value;"
            "var newPassword = document.getElementById('newPassword').value;"
            "var category = document.getElementById('profilecategory').value;"
          "var baseUrl = '/saveProfile';"
          "var url = baseUrl + '?newUserName=' + encodeURIComponent(userName)+'&oldPassword=' + encodeURIComponent(oldPassword) + '&newPassword=' + encodeURIComponent(newPassword) + '&'+ loginParams.toString();"
          
                   // "  fetch(url)"
                    //"    .then(response => response.text())"
                    //"    .then(data => alert(data));"

                    
                   "fetch(url)"
                    ".then(response => {"
                        "if (!response.ok) {"
                            // Handle HTTP error status
                            "throw new Error('HTTP error! Status: ${response.status}');"
                        "}"
                        "return response.text();"
                   " })"
                    ".then(data => alert(data))"
                    ".catch(error => {"
                        // Handle any errors that occurred during the fetch or in the previous then block
                        "alert('Error: ' + error.message);"
                    "});"

                    
        "}"
    "</script>";
}

String CreateVirtualKey()
{
  return ""
    "<h1>Create virtual key</h1>"
    "<form id='virtualkeyForm'>"

        "<label for='code'>Virtual Key Code:</label>"
        "<input type='text' id='keycode' name='keycode' value='' required><br/>"
        "<label for='code'>Remote button number</label>"
        "<input type='number' id='number' name='number' min='1' max='3' required>"

      " <!-- Share via WhatsApp button -->"
        "<button type='button' onclick='createKey()'>Create Virual Key</button>"
        
   "</form><br/>"

  " <script>"
      "function createKey() {"
          "var codeValue = document.getElementById('keycode').value;"
          "var keyNumber = document.getElementById('number').value;"
          "var baseUrl = '/createVirtualKey';"
          "var url = baseUrl + '?key=' + encodeURIComponent(codeValue)+'&remoteNumber='+ encodeURIComponent(keyNumber)+'&'+loginParams.toString();"

        //  "  fetch(url)"
        //  "    .then(response => response.text())"
        //  "    .then(data => alert(data));"

           "fetch(url)"
           " .then(response => {"
                "if (!response.ok) {"
                    // Handle HTTP error status
                    "throw new Error('HTTP error! Status: ${response.status}');"
               " }"
                "return response.text();"
                
           " })"
          ".then(data => {"
              "alert(data);"
              "if (confirm('Request was successful.')) {"
                  "location.reload();"
              "}"
          "})"
          "  .catch(error => {"
                // Handle any errors that occurred during the fetch or in the previous then block
               " alert('Error: ' + error.message);"
            "});"

        "}"
  "</script>";
}

String VirtualKeyShareMessageOptions()
{
  return ""
    "<h1>Customise the Virual Key Message</h1>"
    "<form id='updateCustomeMsgForm'>"

        "<label for='msg'>Custom Message :</label>"
        "<input type='text' id='cusomMsg' name='cusomMsg' value='' required><br/>"

        "<label for='propName'>Andress Or Property Name :</label>"
        "<input type='text' id='propName' name='propName' value='' required><br/>"

        "<label for='locationPin'>Copy and paste location Pin here :</label>"
        "<input type='text' id='locationPin' name='locationPin' value='' required><br/>"

        "<button type='button' onclick='updateCustomeMsg()'>Update Custome Message</button>"
        
   "</form><br/>"

  " <script>"
      "function updateCustomeMsg() {"
          "var CusomMsg = document.getElementById('cusomMsg').value;"
          "var PropName = document.getElementById('propName').value;"
          "var LocationPin = document.getElementById('locationPin').value;"
          "var baseUrl = '/updateCustomMessage';"
          "var url = baseUrl + '?msg=' + encodeURIComponent(CusomMsg)+'&propName=' + encodeURIComponent(PropName) + '&locationPin='+  encodeURIComponent(LocationPin) + '&' + loginParams.toString();"

           "fetch(url)"
           " .then(response => {"
                "if (!response.ok) {"
                    // Handle HTTP error status
                    "throw new Error('HTTP error! Status: ${response.status}');"
               " }"
                "return response.text();"
                
           " })"
          ".then(data => {"
              "alert(data);"
              "if (confirm('Request was successful.')) {"
                  "location.reload();"
              "}"
          "})"
          "  .catch(error => {"
                // Handle any errors that occurred during the fetch or in the previous then block
               " alert('Error: ' + error.message);"
            "});"
        "}"
  "</script>";
}

String GuestLoginPage()
{
  return ""
  "<!DOCTYPE html>"
  "<html lang='en'>"
  "<head>"
      "<meta charset='UTF-8'>"
      "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
      "<title>Login Page with Tabs</title>"
      +style()+
  "</head>"
  "<body>"
    "<div id='tabs'>"
      "<button class='tab' onclick='openTab(\"keyCodeTab\")'>Virtual Key</button>"
      "<button class='tab' onclick='openTab(\"loginTab\")'>login</button>"
    "</div>"
    
    "<div id='loginTab' class='tab-content'>"
        "<h2>Enter Login Details</h2>"
        "<form id='loginForm'>"
            "<label for='loginUsername'>Username:</label>"
            "<input type='text' id='loginUsername' name='loginUsername' required>"

            "<label for='loginUserassword'>Password:</label>"
            "<input type='password' id='loginUserpassword' name='loginUserpassword' required><br/>"

            "<button type='button' onclick='submitLoginForm()'>Submit</button>"
        "</form>"
    "</div>"

    "<div id='keyCodeTab' class='tab-content' >"
        "<h2>Enter Virtual Key Code</h2>"
        "<form id='keyCodeForm'>"
            "<label for='virtualKeyCode'>Virtual Key Code:</label>"
            "<input type='text' id='virtualKeyCode' name='virtualKeyCode' required>"

            "<button type='submit' onclick='submitKeyCodeForm()'>Submit</button>"
        "</form>"
    "</div><br/>"
    "<script>"
        "function openTab(tabName) {"
          "var i, tabContent, tabLinks;"
          "tabContent = document.getElementsByClassName('tab-content');"
          "for (i = 0; i < tabContent.length; i++) {"
            "tabContent[i].style.display = 'none';"
          "}"
          "tabLinks = document.getElementsByClassName('tab');"
          "for (i = 0; i < tabLinks.length; i++) {"
            "tabLinks[i].classList.remove('active-tab');"
          "}"
          "document.getElementById(tabName).style.display = 'block';"
          "event.currentTarget.classList.add('active-tab');"
        "}"

        "function submitLoginForm() {"
          "var username = document.getElementById('loginUsername').value;"
          "var userpassword = document.getElementById('loginUserpassword').value;"
          "var baseUrl = '/login';"
          "var url = baseUrl + '?loginName=' + encodeURIComponent(username) + '&loginPassword=' + encodeURIComponent(userpassword);"
          "fetch(url)"
          "  .then(response => {"
                "if (!response.ok) {"
                "alert('Login failed')"
              "}else{window.location.href = url;}"
              "});"
        "}"

        "function submitKeyCodeForm() {"
          "var code = document.getElementById('virtualKeyCode').value;"
          "var baseUrl = '/useKey';"
          "var url = baseUrl + '?code=' + encodeURIComponent(code);"
          //  "  fetch(url)"
          //  "    .then(response => response.text())"
          //  "    .then(data => alert(data));"
            
           "fetch(url)"
           " .then(response => {"
                "if (!response.ok) {"
                    // Handle HTTP error status
                    "throw new Error('HTTP error! Status: ${response.status}');"
               " }"
                "return response.text();"
                
           " })"
          ".then(data => {"
              "alert(data);"
              "if (confirm('Request was successful.')) {"
                  "location.reload();"
              "}"
          "})"
          "  .catch(error => {"
                // Handle any errors that occurred during the fetch or in the previous then block
               " alert('Error: ' + error.message);"
            "});"
        "}"

        // Open the Virtual Key tab by default
        "openTab('keyCodeTab');"
    "</script>"
  "</body>"
  "</html>";
}

//main pages
String StreamTab()
{
   return  
   "<img id=\"streamImage\" alt=\"Streaming Image\" ><br/>"
    "<script>"
     "document.getElementById('streamImage').src = window.location.protocol + '//' + window.location.hostname + ':81/stream';"
    "</script>"
    
    "<script>"
        "document.addEventListener('DOMContentLoaded', function() {"
            // Get the image element
            "var streamImage = document.getElementById('streamImage');"

            // Set up variables for motion detection
            "var previousImageData;"

            // Function to check for motion
            "function detectMotion() {"
                "var canvas = document.createElement('canvas');"
                "var ctx = canvas.getContext('2d');"

                // Ensure that the image has been fully loaded
                "if (streamImage.complete) {"

                  // Set the canvas size to match the image size
                  "canvas.width = streamImage.width;"
                  "canvas.height = streamImage.height;"

                  // Draw the image onto the canvas
                  "ctx.drawImage(streamImage, 0, 0, canvas.width, canvas.height);"

                  // Get the current image data
                  "var currentImageData = ctx.getImageData(0, 0, canvas.width, canvas.height).data;"

                  // Compare with previous image data (if available)
                  "if (previousImageData) {"
                      "for (var i = 0; i < currentImageData.length; i++) {"
                          // Check for a significant difference in pixel values
                          "if (Math.abs(currentImageData[i] - previousImageData[i]) > 10) {"
                              // Motion detected, you can add your own logic here
                              "console.log('Motion detected!');"
                              "break;"
                          "}"
                      "}"
                  "}"

                  // Update previous image data
                  "previousImageData = currentImageData;"
                "}"
                // Continue detecting motion in the next animation frame
                "requestAnimationFrame(detectMotion);"
            "}"

            // Start the motion detection loop
            "detectMotion();"
        "});"
    "</script>";
}
    
String tabAdmin(String wifiName,String portalName,String url,bool wifiConnected,String msg,String domainName)
{
  String SDSection ="";
  if(!sdActive)
  {
    SDSection += "<p>SD Card did not innitiate proparly</p>"
    
        "<button type='button' onclick='readSD()'>Ensure the card is inserted and re-Load.</button>"
        "<button type='button' onclick='restartDevice()'>Reset device to initiate SD card.</button>";
  }
  String wifiSection ="";
  if(wifiConnected){
    wifiSection += "<p> Wi-Fi is connected and local server is running</p>"

                    "<p id='domainNameDisplay'>Domain Name: '"+domainName+"'</p>"
                    "<a href='"+domainName+"' id='serverUrlDisplay' target='_blank'>Navigate to your camera feed on WiFi network using Domain Name.</a>"

                    "<p id='serverUrlDisplay'>IP Address: '"+url+"'</p>"
                    "<a href='"+url+"' id='serverUrlDisplay' target='_blank'>Navigate to your camera feed on WiFi network using IP Addredss.</a>";

  }
  else{
    wifiSection+="<p> WiFi is not connected</p><br/>";
  }

  String portalSection ="";
  if(GuestMode)
  {
    portalSection+= "<p>Guest Mode is activated. </p>";
  }
  else{
    portalSection+= "<p>Guest Mode is not activated.</p>";
    if(wifiConnected)
    {
      portalSection+= "<p>Note that the Wifi server is running, anyone who has access to the network will have admin privilages on you Skelleton key. <br/> It is advised to activate guest mode if the Wifi server is running.</p>"
                      "<a href='#' onclick='openTab(\"tabPortalForm\")'>Go to Portal Settings</a>";
    }
  }
  
  return ""+SDSection+
    "<h1>Setup Information</h1>"
    "<h3>Wifi Info</h3>"
    "<p id='ssidDisplay'>Local Wi-Fi Network : '"+wifiName+"'</p>"
    +wifiSection+
    "<h3>Portal info</h3>"
    "<p>Portal Name: '"+portalName+"'</p>"
    +portalSection+

    "<br/><h2>Custom Message Info</h2>"
    "<p id='CustomMsgDislay'>Custome Message: '"+CostomeMessage+"'</p>"
    "<p id='LocationNameDislay'>Location Name: '"+LocationName+"'</p>"
    "<p id='LocationPinDislay'>Where: '"+LocationPin+"'</p>"
    
      "<script>"
          "function restartDevice() {"
            // "var baseUrl = '/portal/connect/';"
              "var baseUrl = '/reset?'+ loginParams.toString();"
              "var url = baseUrl;"
              
            "fetch(url)"
            " .then(response => {"
                  "if (!response.ok) {"
                      // Handle HTTP error status
                      "throw new Error('HTTP error! Status: ${response.status}');"
                " }"
                  "return response.text();"
                  
            " })"
            ".then(data => {"
                "alert(data);"
                "if (confirm('Request was successful.')) {"
                    "location.reload();"
                "}"
            "})"
            ".catch(error => {"
                  // Handle any errors that occurred during the fetch or in the previous then block
                " alert('Error: ' + error.message);"
              "});"
          "}"
          "function readSD() {"
            // "var baseUrl = '/portal/connect/';"
              "var baseUrl = '/readSD?'+ loginParams.toString();"
              "var url = baseUrl;"
              
            "fetch(url)"
            " .then(response => {"
                  "if (!response.ok) {"
                      // Handle HTTP error status
                      "throw new Error('HTTP error! Status: ${response.status}');"
                " }"
                  "return response.text();"
                  
            " })"
            ".then(data => {"
                "alert(data);"
                "if (confirm('Request was successful.')) {"
                    "location.reload();"
                "}"
            "})"
            ".catch(error => {"
                  // Handle any errors that occurred during the fetch or in the previous then block
                " alert('Error: ' + error.message);"
              "});"
          "}"
      "</script>";
}

String tabPortalForm (String portalName,String portalPassword)
{
    return ""
      "<h1>Portal Server</h1>"
      "<form id='portalForm'>"
          "<label for='pname'>Portal Name:</label>"
          "<input type='text' id='pname' name='pname' value='"+portalName+"' required><br/>"

          "<label for='ppassword'>Password:</label>"
          "<input type='password' id='ppassword' name='ppassword' value='"+portalPassword+"' required><br/><br/>"

          "<label for='guestMode'>Run in guest Mode:</label>"
          "<input type='checkbox' id='guestMode' name='guestMode'><br/>"

          "<button type='button' onclick='savePortalCredentials()'>Save</button>"
      "</form><br/>"

      "<form id='restartAP'>"
        "<button type='button' onclick='restartPortal()'>Restart portal with new settings.</button>"
      "</form><br/>"
      "<script>"
      
          "function togglePasswordInput() {"
              "var ppasswordInput = document.getElementById('ppassword');"
              "var guestModeCheckbox = document.getElementById('guestMode');"
              "ppasswordInput.disabled = guestModeCheckbox.checked;"
          "}"
          
          "function savePortalCredentials() {"
              "var pName = document.getElementById('pname').value;"
              "var pPassword = document.getElementById('ppassword').value;"
              "var guestMode = document.getElementById('guestMode').value;"
              "var pPassword = document.getElementById('ppassword').value;"
              // "var baseUrl = '/portal/saveportal';"saveportal
              "var baseUrl = '/saveportal';"
              "var url = baseUrl  + '?pName=' + encodeURIComponent(pName) + '&pPassword=' + encodeURIComponent(pPassword)+ '&guestMode='+encodeURIComponent(guestMode)+'&'+ loginParams.toString();"

                    //"  fetch(url)"
                    //"    .then(response => response.text())"
                    //"    .then(data => alert(data));"

                    
           "fetch(url)"
           " .then(response => {"
                "if (!response.ok) {"
                    // Handle HTTP error status
                    "throw new Error('HTTP error! Status: ${response.status}');"
               " }"
                "return response.text();"
                
           " })"
          ".then(data => {"
              "alert(data);"
              "if (confirm('Request was successful.')) {"
                  "location.reload();"
              "}"
          "})"
          "  .catch(error => {"
                // Handle any errors that occurred during the fetch or in the previous then block
               " alert('Error: ' + error.message);"
            "});"
        
          "}"
          "function restartPortal() {"
            // "var baseUrl = '/portal/connect/';"
              "var baseUrl = '/restartPortal?'+ loginParams.toString();"
              "var url = baseUrl;"
              
              //      "  fetch(url)"
              //      "    .then(response => response.text())"
              //      "    .then(data => alert(data));"
              //"location.reload();"

            
           "fetch(url)"
           " .then(response => {"
                "if (!response.ok) {"
                    // Handle HTTP error status
                    "throw new Error('HTTP error! Status: ${response.status}');"
               " }"
                "return response.text();"
                
           " })"
          ".then(data => {"
              "alert(data);"
              "if (confirm('Request was successful.')) {"
                  "location.reload();"
              "}"
          "})"
          "  .catch(error => {"
                // Handle any errors that occurred during the fetch or in the previous then block
               " alert('Error: ' + error.message);"
            "});"
        
              
          "}"
      "</script>";
}

String tabWifiForm(String wifiName,String WifiPass,String domainName)
{
  return ""
      "<h1>WiFi Server</h1>"
      "<form id='wifiForm'>"
          "<label for='ssid'>Wifi Name / SSID:</label>"
          "<input type='text' id='ssid' name='ssid' value='"+wifiName+"' required><br/>"

          "<label for='domainName'>Domain Name:</label>"
          "<input type='text' id='domainName' name='domainName' value='"+domainName+"' required><br/><br/>"

          "<label for='password'>Password:</label>"
          "<input type='password' id='password' name='password' value='"+WifiPass+"' required><br/><br/>"

          "<button type='button' onclick='saveCredentials()'>Save</button>"
          "<details>"
            "<summary>Instructions</summary>"
            "<p>This page enables your Skeleton Key to function over your local Wi-Fi network.<br/>"
            "Please use the form above to input your local Wi-Fi network login details.<br/>"
            "The domain name you enter above will create a clickable link usable directly from WhatsApp when sharing a virtual key.<br/>"
            "To ensure the virtual key operates correctly, you must create a DNS record on your router, linking the domain with the IP address of the device. This helps redirect your virtual key link to the Skeleton Key device.<br/>"
            "If your domain name is not registered on your router, the virtual key recipient will need to log in to the Skeleton Key via the Portal Network. Ensure that authentication is enabled on the Portal Settings page to prevent unauthorized access to administrative functions.</p>"
          "</details>"
      "</form><br/>"

      "<form id='connect'>"
        "<button type='button' onclick='connectWiFi()'>Connect to WiFi and start server</button>"
      "</form><br/>"
      "<script>"
        "function saveCredentials() {"
            "var ssid = document.getElementById('ssid').value;"
            "var domainName = document.getElementById('domainName').value;"
            "var password = document.getElementById('password').value;"
            "var baseUrl = '/savewifi';"
            "var url = baseUrl + '?ssid=' + encodeURIComponent(ssid) + '&domainName=' + encodeURIComponent(domainName) +'&password=' + encodeURIComponent(password) + '&'+ loginParams.toString();"

                  //  "  fetch(url)"
                  //  "    .then(response => response.text())"
                  //  "    .then(data => alert(data));"
                    
           "fetch(url)"
           " .then(response => {"
                "if (!response.ok) {"
                    // Handle HTTP error status
                    "throw new Error('HTTP error! Status: ${response.status}');"
               " }"
                "return response.text();"
                
           " })"
          ".then(data => {"
              "alert(data);"
              "if (confirm('Request was successful.')) {"
                  "location.reload();"
              "}"
          "})"
          "  .catch(error => {"
                // Handle any errors that occurred during the fetch or in the previous then block
               " alert('Error: ' + error.message);"
            "});"
        
        "}"
        "function connectWiFi() {"
          // "var baseUrl = '/portal/connect/';"
            "var baseUrl = '/connectx?'+ loginParams.toString();;"
            "var url = baseUrl;"
            
                    "  fetch(url)"
                    "    .then(response => response.text())"
                    "    .then(data => alert(data));"
        "}"
      "</script>";
}

String combinedPage(String wifiName,String WifiPass,String portalName,String portalPassword,String url,bool wifiConnected,String msg,String domainName)
{
  return ""
    
  "<!DOCTYPE html>"
  "<html lang='en'>"
  "<head>"
      "<meta charset='UTF-8'>"
      "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
      "<title>Tab page</title>"
      +style()+
  "</head>"
  "<body>"
    "<h1>SK Portal</h1>"
        "<!-- Menu container with options -->"
        "<div class='dropdown' style='position:fixed; top: 5px; right: 0px;'>"
          "<button >Menu &#9662;</button>"
          "<div class='dropdown-content'>"
            "<a href='#' onclick='openTab(\"updateCustomMessageForm\")'>Message Settup  </a>"
            "<a href='#' onclick='openTab(\"profile\")'>Profile  </a>"
            "<a href='#' onclick='openTab(\"tabWifiForm\")'>Wifi Settings  </a>"
            "<a href='#' onclick='openTab(\"tabPortalForm\")'>Portal Settings</a>"
          "</div>"
        "</div>"

    "<div id='tabs'>"
      "<button class='tab' onclick='openTab(\"streamTab\")'>Stream</button>"
      "<button class='tab' onclick='openTab(\"tabControl\")'>Controls</button>"
      "<button class='tab' onclick='openTab(\"keysTab\")'>Keys</button>"
      "<button class='tab' onclick='openTab(\"usersTab\")'>Users</button>"
    "</div>"

    
    "<div class='container'>"
        "<div class='left-column'>"
          "<div id='tabControl' class='tab-content'>"
            + tabControl() +
          "</div>"
          "<div id='keysTab' class='tab-content'>"
            + KeyPage(url, msg, domainName) +
          "</div>"
          "<div id='usersTab' class='tab-content'>"
            + UserPage() +
          "</div>"
          "<div id='tabWifiForm' class='tab-content'>"
            + tabWifiForm(wifiName,WifiPass,domainName) +
          "</div>"
          "<div id='tabPortalForm' class='tab-content'>"
            + tabPortalForm(portalName,portalPassword) +
          "</div>"
          "<div id='userForm' class='tab-content'>"
            + newUserForm() +
          "</div>"
          "<div id='updateCustomMessageForm' class='tab-content'>"
            + VirtualKeyShareMessageOptions() +
          "</div>"
          "<div id='keyPage' class='tab-content'>"
            + CreateVirtualKey() +
          "</div>"
          "<div id='profile' class='tab-content'>"
            + userProfileForm() +
          "</div>"
          "<div id='streamTab' class='tab-content'>"
          "</div>"
          
        "</div>"
        "<div class='right-column'>"
            + StreamTab() +
        "</div>"
    "</div>"
     + tabAdmin(wifiName,portalName,url,wifiConnected,msg,domainName) +

      "<script>"
      "const loginParams = new URLSearchParams(window.location.search);"
      
        "function openTab(tabName) {"
          "var i, tabContent, tabLinks;"
          "tabContent = document.getElementsByClassName('tab-content');"
          "for (i = 0; i < tabContent.length; i++) {"
            "tabContent[i].style.display = 'none';"
          "}"
          "tabLinks = document.getElementsByClassName('tab');"
          "for (i = 0; i < tabLinks.length; i++) {"
            "tabLinks[i].classList.remove('active-tab');"
          "}"
          "document.getElementById(tabName).style.display = 'block';"
          "event.currentTarget.classList.add('active-tab');"
        "}"
      "</script>"
      +style()+
      // Make tabControl active by default
      "<script>openTab('tabControl');</script>"
      "<script>"
          // Function to toggle the menu visibility
        " function toggleMenu() {"
            " var menu = document.getElementById('menu');"
            " menu.style.display = (menu.style.display === 'block') ? 'none' : 'block';"
          "}"

      "</script>"

    "</body>"
    "</html>";
}
