package com.example.crowdcontrol

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.Toast
import androidx.core.text.isDigitsOnly
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import com.example.crowdcontrol.databinding.ActivityMainBinding
import kotlinx.coroutines.*
var check=true;
var check1=true;
class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        var binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        getSupportActionBar()?.hide()


        var model=ViewModelProvider(this).get(MainActivityModel::class.java)
        model.maxppl.observe(this, Observer {
            binding.tvMax.setText("Max people Limit: "+it)
        })

        model.currentppl.observe(this, Observer {
            var prev=binding.tvPpl.text.toString().toInt()
            binding.tvPpl.setText(it.toString())
            if(model.currentppl.value!! >= model.maxppl.value!!)
            binding.tvLimit.text = "Limit Exceeded !"
            else
            binding.tvLimit.text = ""
if(!check && !check1) {
    if (prev > binding.tvPpl.text.toString().toInt())
        Toast.makeText(this, "Person Left", Toast.LENGTH_SHORT).show()
    else if (prev < binding.tvPpl.text.toString().toInt())
        Toast.makeText(this, "Person Entered", Toast.LENGTH_SHORT).show()
}
            if(!check){
                check1=false;
            }
            check=false;
        })
        CoroutineScope(Dispatchers.IO).launch{
            async{
                while(true) {
                    model.Fetch()
                    delay(100);
                }
            }
        }



        /* on changing the max limit */
        binding.changBtn.setOnClickListener()
        {
            if(binding.changeView.text.toString() == "")
                return@setOnClickListener
            else if(!binding.changeView.text.isDigitsOnly())
                return@setOnClickListener

            var limit:Int = binding.changeView.text.toString().toInt()
            model.db.getReference().child("limit").setValue(limit)
        }
    }
}