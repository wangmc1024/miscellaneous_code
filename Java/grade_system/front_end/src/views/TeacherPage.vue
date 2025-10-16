<template>
  <div class="teacher-grade-container">
    <h2>学生成绩管理</h2>
    <div class="search-bar">
      <input v-model="searchName" placeholder="输入姓名查找学生" />
      <button @click="searchStudent">查找</button>
      <button @click="resetSearch" class="reset-btn">重置</button>
    </div>
    <div class="table-wrapper">
      <table>
        <thead>
          <tr>
            <th>班级</th>
            <th>姓名</th>
            <th>平时分</th>
            <th>考试成绩</th>
            <th>总成绩</th>
            <th>操作</th>
          </tr>
        </thead>
        <tbody>
          <tr v-for="(student, idx) in filteredStudents" :key="student.name + student.class">
            <td>
              <input v-if="editIndex === idx" v-model="editStudent.class" class="edit-input"/>
              <span v-else>{{ student.class }}</span>
            </td>
            <td>
              <input v-if="editIndex === idx" v-model="editStudent.name" class="edit-input"/>
              <span v-else>{{ student.name }}</span>
            </td>
            <td>
              <input v-if="editIndex === idx" type="number" v-model.number="editStudent.usualScore" class="edit-input"/>
              <span v-else>{{ student.usualScore }}</span>
            </td>
            <td>
              <input v-if="editIndex === idx" type="number" v-model.number="editStudent.examScore" class="edit-input"/>
              <span v-else>{{ student.examScore }}</span>
            </td>
            <td>{{ student.usualScore + student.examScore }}</td>
            <td>
              <div class="action-btns">
                <button v-if="editIndex === idx" @click="saveEdit(idx)" class="save-btn">保存</button>
                <button v-if="editIndex === idx" @click="cancelEdit" class="cancel-btn">取消</button>
                <button v-else @click="startEdit(idx, student)" class="edit-btn">编辑</button>
                <button @click="deleteStudent(idx)" class="delete-btn">删除</button>
              </div>
            </td>
          </tr>
          <tr v-if="filteredStudents.length === 0">
            <td colspan="6" class="empty-row">暂无数据</td>
          </tr>
        </tbody>
      </table>
    </div>
    <div class="add-form">
      <h3>添加学生成绩</h3>
      <input v-model="newStudent.class" placeholder="班级"/>
      <input v-model="newStudent.name" placeholder="姓名" />
      <input type="number" v-model.number="newStudent.usualScore" placeholder="平时分" />
      <input type="number" v-model.number="newStudent.examScore" placeholder="考试成绩" />
      <button @click="addStudent" class="add-btn">添加</button>
    </div>
  </div>
</template>

<script setup>
import { ref, computed } from 'vue'

const students = ref([
  { class:'2302',name: '张三', usualScore: 30, examScore: 60 },
  { class:'2302',name: '李四', usualScore: 35, examScore: 55 },
  { class:'2304',name: '王五', usualScore: 28, examScore: 65 }
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

const newStudent = ref({ class:'',name: '', usualScore: '', examScore: '' })
function addStudent() {
  if (!newStudent.value.class) return alert('请输入班级')
  if (!newStudent.value.name) return alert('请输入姓名')
  students.value.push({ ...newStudent.value })
  newStudent.value = { class:'', name: '', usualScore: 0, examScore: 0 }
}

const editIndex = ref(-1)
const editStudent = ref({ class:'', name: '', usualScore: 0, examScore: 0 })

function startEdit(idx, student) {
  editIndex.value = idx
  editStudent.value = { ...student }
}
function saveEdit(idx) {
  students.value[idx] = { ...editStudent.value }
  editIndex.value = -1
}
function cancelEdit() {
  editIndex.value = -1
}
function deleteStudent(idx) {
  if (confirm('确定要删除该学生成绩吗？')) {
    students.value.splice(idx, 1)
  }
}
</script>

<style scoped>
 /* input::-webkit-outer-spin-button, input::-webkit-inner-spin-button {
   -webkit-appearance: none; margin: 0;
  }

   input[type="number"] {
    -moz-appearance: textfield;
  } */

.teacher-grade-container {
  width: 80%;
  height: auto;
  margin: 40px auto;
  padding: 32px 28px 28px 28px;
  background: #f5f7fa;
  border-radius: 12px;
  box-shadow: 0 4px 24px #e0e7ef;
  font-family: 'Segoe UI', 'PingFang SC', Arial, sans-serif;
}
h2 {
  text-align: center;
  margin-bottom: 28px;
  color: #2d3a4b;
  letter-spacing: 2px;
}
.search-bar {
  margin-bottom: 18px;
  display: flex;
  gap: 10px;
  justify-content: flex-start;
  align-items: center;
}
.search-bar input {
  padding: 7px 12px;
  border: 1px solid #bfcbd9;
  border-radius: 5px;
  font-size: 15px;
  width: 220px;
}
.search-bar button {
  padding: 7px 18px;
  border: none;
  border-radius: 5px;
  background: #409eff;
  color: #fff;
  font-size: 15px;
  cursor: pointer;
  transition: background 0.2s;
}
.search-bar .reset-btn {
  background: #e6a23c;
}
.search-bar button:hover {
  background: #307ae8;
}
.search-bar .reset-btn:hover {
  background: #cf9236;
}
.table-wrapper {
  overflow-x: auto;
  background: #fff;
  border-radius: 8px;
  box-shadow: 0 1px 6px #e0e7ef;
  margin-bottom: 28px;
}
table {
  width: 100%;
  border-collapse: collapse;
  background: #fff;
  min-width: 700px;
}
th, td {
  border: 1px solid #e5e9f2;
  padding: 12px 8px;
  text-align: center;
  font-size: 15px;
}
th {
  background: #f0f4fa;
  color: #34495e;
  font-weight: 600;
}
.edit-input {
  width: 90%;
  padding: 5px 8px;
  border: 1px solid #bfcbd9;
  border-radius: 4px;
  font-size: 15px;
}
.action-btns {
  display: flex;
  gap: 4px;
  justify-content: center;
}
button {
  padding: 5px 12px;
  border: none;
  border-radius: 4px;
  font-size: 14px;
  cursor: pointer;
  transition: background 0.2s;
}
.edit-btn {
  background: #67c23a;
  color: #fff;
}
.edit-btn:hover {
  background: #4ea43a;
}
.save-btn {
  background: #409eff;
  color: #fff;
}
.save-btn:hover {
  background: #307ae8;
}
.cancel-btn {
  background: #e6a23c;
  color: #fff;
}
.cancel-btn:hover {
  background: #cf9236;
}
.delete-btn {
  background: #f56c6c;
  color: #fff;
}
.delete-btn:hover {
  background: #d9534f;
}
.empty-row {
  color: #aaa;
  font-style: italic;
  background: #fafbfc;
}
.add-form {
  margin-top: 32px;
  display: flex;
  justify-content: center;
  gap: 10px;
  align-items: center;
  background: #fff;
  padding: 18px 16px;
  border-radius: 8px;
  box-shadow: 0 1px 6px #e0e7ef;
}
.add-form h3 {
  margin: 0 12px 0 0;
  font-size: 17px;
  color: #2d3a4b;
  font-weight: 500;
}
.add-form input {
  padding: 7px 10px;
  border: 1px solid #bfcbd9;
  border-radius: 4px;
  font-size: 15px;
  width: 120px;
}
.add-form .add-btn {
  padding: 7px 20px;
  background: #409eff;
  color: #fff;
  border: none;
  border-radius: 5px;
  font-size: 15px;
  cursor: pointer;
  transition: background 0.2s;
}
.add-form .add-btn:hover {
  background: #307ae8;
}
@media (max-width: 700px) {
  .teacher-grade-container {
    padding: 10px 2vw;
  }
  .add-form {
    flex-direction: column;
    align-items: stretch;
    gap: 8px;
  }
  .add-form h3 {
    margin-bottom: 6px;
  }
  .search-bar {
    flex-direction: column;
    gap: 8px;
    align-items: stretch;
  }
  table {
    min-width: 500px;
    font-size: 13px;
  }
}
</style>
