<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>登录失败</title>
    <style>
        body {
            background: linear-gradient(120deg, #fbc2eb 0%, #a6c1ee 100%);
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
            color: #d7263d;
            margin-bottom: 16px;
        }
        a {
            display: inline-block;
            padding: 8px 24px;
            background: #d7263d;
            color: #fff;
            border-radius: 6px;
            text-decoration: none;
            transition: background 0.2s;
        }
        a:hover {
            background: #ffb6b9;
        }
    </style>
</head>
<body>
    <div class="container">
        <h3>登录失败！用户名或密码错误。</h3>
        <a href="login.jsp">重新登录</a>
    </div>
</body>
</html>