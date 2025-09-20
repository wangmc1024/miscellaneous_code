<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>登录</title>
    <style>
        body {
            background: linear-gradient(120deg, #fdfbfb 0%, #ebedee 100%);
            font-family: 'Segoe UI', Arial, sans-serif;
            margin: 0;
            padding: 0;
        }
        .container {
            max-width: 400px;
            margin: 80px auto;
            background: #fff;
            border-radius: 12px;
            box-shadow: 0 4px 24px rgba(0,0,0,0.12);
            padding: 32px 24px;
            text-align: center;
        }
        h3 {
            color: #4e54c8;
            margin-bottom: 24px;
        }
        input[type="text"], input[type="password"] {
            width: 80%;
            padding: 10px;
            margin: 8px 0 16px 0;
            border: 1px solid #ccc;
            border-radius: 6px;
            font-size: 1em;
        }
        input[type="submit"] {
            padding: 10px 32px;
            background: #4e54c8;
            color: #fff;
            border: none;
            border-radius: 6px;
            font-size: 1em;
            cursor: pointer;
            transition: background 0.2s;
        }
        input[type="submit"]:hover {
            background: #6a82fb;
        }
        label {
            display: block;
            margin-bottom: 6px;
            color: #333;
            text-align: left;
            width: 80%;
            margin-left: 10%;
        }
    </style>
</head>
<body>
    <div class="container">
        <h3>用户登录</h3>
        <form action="login" method="post">
            <label for="username">用户名：</label>
            <input type="text" name="username" id="username" required>
            <label for="password">密码：</label>
            <input type="password" name="password" id="password" required>
            <input type="submit" value="登录">
        </form>
    </div>