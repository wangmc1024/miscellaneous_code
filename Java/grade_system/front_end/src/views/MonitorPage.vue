<template>
  <div class="monitor-container">
    <!-- 个人成绩区域 -->
    <div class="personal-grade">
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

    <!-- 班级成绩区域 -->
    <div class="class-grade">
      <h2>本班同学成绩</h2>
      <div class="search-bar">
        <input 
          v-model="searchName"
          placeholder="输入姓名搜索"
          @keyup.enter="searchStudent"
        />
        <button @click="searchStudent">搜索</button>
        <button class="reset-btn" @click="resetSearch">重置</button>
      </div>
      
      <div class="table-wrapper">
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
            <tr v-for="(s, index) in filteredStudents" :key="index">
              <td>{{ s.name }}</td>
              <td>{{ s.usualScore }}</td>
              <td>{{ s.examScore }}</td>
              <td>{{ s.usualScore + s.examScore }}</td>
            </tr>
            <tr v-if="filteredStudents.length === 0" class="empty-row">
              <td colspan="4">暂无数据</td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, computed } from 'vue'

// 个人成绩数据
const student = ref({
  name: '班长姓名',
  usualScore: 40,
  examScore: 70
})

const totalScore = computed(() => student.value.usualScore + student.value.examScore)

// 班级成绩数据
const students = ref([
  { name: '同学A', usualScore: 30, examScore: 60 },
  { name: '同学B', usualScore: 35, examScore: 55 },
  { name: '同学C', usualScore: 28, examScore: 65 }
])

const searchName = ref('')
const filteredStudents = computed(() => {
  if (!searchName.value) return students.value
  return students.value.filter(s => s.name.includes(searchName.value))
})

function searchStudent() {}
function resetSearch() {
  searchName.value = ''
}
</script>

<style scoped>
.monitor-container {
  max-width: 900px;
  margin: 40px auto;
  padding: 32px;
  background: #f5f7fa;
  border-radius: 12px;
  box-shadow: 0 4px 24px #e0e7ef;
}

.personal-grade {
  margin-bottom: 40px;
  padding: 24px;
  background: #fff;
  border-radius: 8px;
  box-shadow: 0 1px 6px #e0e7ef;
}

.class-grade {
  padding: 24px;
  background: #fff;
  border-radius: 8px;
  box-shadow: 0 1px 6px #e0e7ef;
}

h2 {
  text-align: center;
  margin-bottom: 24px;
  color: #2d3a4b;
}

table {
  width: 100%;
  border-collapse: collapse;
  margin-top: 16px;
}

th, td {
  border: 1px solid #e5e9f2;
  padding: 12px 8px;
  text-align: center;
}

th {
  background: #f0f4fa;
}

.search-bar {
  margin-bottom: 16px;
  display: flex;
  gap: 8px;
}

.search-bar input {
  padding: 8px 12px;
  border: 1px solid #bfcbd9;
  border-radius: 4px;
  width: 200px;
}

.search-bar button {
  padding: 8px 16px;
  border: none;
  border-radius: 4px;
  background: #409eff;
  color: #fff;
  cursor: pointer;
}

.search-bar .reset-btn {
  background: #e6a23c;
}

.empty-row {
  color: #aaa;
  font-style: italic;
}
</style>
