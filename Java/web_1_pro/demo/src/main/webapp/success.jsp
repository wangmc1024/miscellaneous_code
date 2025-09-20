<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ page import="com.example.bean.User" %>
<html>
<head>
    <title>登录成功</title>
    <style>
        body {
            background: linear-gradient(120deg, #a1c4fd 0%, #c2e9fb 100%);
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
            margin-bottom: 16px;
        }
        p {
            font-size: 1.1em;
            color: #333;
            margin-bottom: 24px;
        }
        a {
            display: inline-block;
            padding: 8px 24px;
            background: #4e54c8;
            color: #fff;
            border-radius: 6px;
            text-decoration: none;
            transition: background 0.2s;
        }
        a:hover {
            background: #6a82fb;
        }
    </style>
</head>
<body>
    <div class="container">
        <h3>登录成功！</h3>
        <%
            // 从 session 中获取用户对象
            User user = (User) session.getAttribute("user");
        %>
        <p>欢迎您，<%= user.getUsername() %></p>
        <a href="login.jsp">返回登录</a>
    </div>
</body>
</html>