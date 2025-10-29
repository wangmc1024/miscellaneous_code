<template>
  <div class="student-grade-container">
    <h2>我的成绩</h2>
    <table>
      <thead>
        <tr>
          <th>姓名</th>
          <th>平时分</th>
          <th>考试成绩</th>
          <th>总成绩</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td>{{ student.name }}</td>
          <td>{{ student.usualScore }}</td>
          <td>{{ student.examScore }}</td>
          <td>{{ totalScore }}</td>
        </tr>
      </tbody>
    </table>
  </div>
</template>

<script setup>
import { ref, computed } from 'vue'

// 学生信息
const student = ref({
  name: '李四',
  usualScore: 30,
  examScore: 68
})

const totalScore = computed(() => student.value.usualScore + student.value.examScore)
</script>

<style scoped>
.student-grade-container {
  max-width: 400px;
  margin: 40px auto;
  padding: 24px;
  background: #f9f9f9;
  border-radius: 8px;
  box-shadow: 0 2px 8px #eee;
}
h2 {
  text-align: center;
  margin-bottom: 24px;
}
table {
  width: 100%;
  border-collapse: collapse;
  background: #fff;
}
th, td {
  border: 1px solid #ddd;
  padding: 10px 8px;
  text-align: center;
}
th {
  background: #f0f4fa;
}
</style>
