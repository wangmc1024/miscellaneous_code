<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>简单信息处理</title>
</head>
<body>
    <form action="" method="post">
        姓名：<input type="text" name="name"><br>
        年龄：<input type="text" name="age"><br>
        <input type="submit" value="提交">
    </form>
    <%
        request.setCharacterEncoding("UTF-8");
        String name = request.getParameter("name");
        String age = request.getParameter("age");
        if (name != null && age != null) {
    %>
        <h3>您输入的信息：</h3>
        姓名：<%= name %><br>
        年龄：<%= age %><br>
    <%
        }
    %>
</body>
</html>