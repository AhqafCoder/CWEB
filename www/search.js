
function searchStudents() {
    const input = document.getElementById('search-input').value.toLowerCase();
    const rows = document.querySelectorAll('#student-table tbody tr');
    rows.forEach(row => {
        const studentData = row.textContent.toLowerCase();
        row.style.display = studentData.includes(input) ? '' : 'none';
    });
}
