#! /bin/bash
#代码书行数统计脚本，只支持两级目录
all_count=0; 	#全部行数统计
file_count=0; 	#文件行数统计
filename="";
string="";
path=`pwd`;
for filename in `ls`;do
	file_info=`ls -l | grep ${filename}`;
	class=`echo ${file_info:0:1}`;
	#目录
	if [[ ${class} = 'd' ]];then
		cd ${filename};
		path=`pwd`;
		for filename in `ls`; do
			file_info=`ls -l | grep ${filename}`;
			class=`echo ${file_info:0:1}`;
			
			#第1级目录
			if [[ ${class} = 'd' ]]; then
				cd ${filename};
				path=`pwd`;
					
					for filename in `ls`; do
						file_info=`ls -l | grep ${filename}`;
						class=`echo ${file_info:0:1}`;
						#第2级目录
						if [[ ${class} = "d" ]]; then
						#do nothing
							echo "";
						#第2级文件
						else
							string=`wc -l ${filename}`;
							file_count=`echo ${string} |cut -d' ' -f 1`;
							echo "line_count:${file_count}		filename:${path}/${filename}";
							let  "all_count=file_count+all_count";
						fi
					done
					
				cd ..;
				path=`pwd`;
			#第1级文件
			else
				string=`wc -l ${filename}`;
				file_count=`echo ${string} |cut -d' ' -f 1`;
				echo "line_count:${file_count}		filename:${path}/${filename}";
				let  "all_count=file_count+all_count";
			fi	
		done
		
		cd ..;
		path=`pwd`;
	#文件
	else
		string=`wc -l ${filename}`;
		file_count=`echo ${string} |cut -d' ' -f 1`;
		echo "line_count:${file_count}		filename:${path}/${filename}";
		let  "all_count=file_count+all_count";
	fi
	
	
done
echo "total_line_count:${all_count}";
