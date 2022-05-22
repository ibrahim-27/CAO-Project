package com.example.crowdcontrol

import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener
import kotlin.math.max

class MainActivityModel: ViewModel() {
    var maxppl:MutableLiveData<Int> = MutableLiveData()
    var currentppl:MutableLiveData<Int> = MutableLiveData()
    var db=FirebaseDatabase.getInstance()
    fun Fetch(){
        db.getReference("Total").addListenerForSingleValueEvent(object :ValueEventListener{
            override fun onDataChange(snapshot: DataSnapshot) {
                currentppl.value=snapshot.value.toString().toInt()
            }

            override fun onCancelled(error: DatabaseError) {
                TODO("Not yet implemented")
            }

        })
        db.getReference("limit").addListenerForSingleValueEvent(object :ValueEventListener{
            override fun onDataChange(snapshot: DataSnapshot) {
                maxppl.value=snapshot.value.toString().toInt()
            }

            override fun onCancelled(error: DatabaseError) {
                TODO("Not yet implemented")
            }

        })
    }
    init{
        maxppl.value=0
        currentppl.value=0
    }
}