import re
import json
import os
basic_kernel_info_2_array = []
qtuser3d_info_2_array = []
shader_entity_info_2_array = []
other_info_2_array = []

def filter_valid_info(file_path, output_path):
    valid_info_pattern_start = re.compile(r'new_scalar.cpp')
    valid_info_pattern_end = re.compile(r'moc_')
    valid_info_pattern_end2 = re.compile(r'exe_common.inl')
    valid_info_end3 = re.compile(r'ntdll.dll!')
    global basic_kernel_info_2_array
    global other_info_2_array
    global qtuser3d_info_2_array
    global shader_entity_info_2_array
    valid_data = []
    isValid = False
    filter_string = '    Qt5'
    tmp_2_array = []
    with open(file_path, 'r', encoding='utf-8') as infile, \
        open(output_path, 'w', encoding='utf-8') as outfile:
        for line in infile:
            if(filter_string in line):
                continue
            if(isValid) : 
                valid_data.append(line)
            if valid_info_pattern_start.search(line):
                isValid = True
                valid_data.append(line)
                if("BasicKernel.dll" in line):
                    tmp_2_array = basic_kernel_info_2_array
                elif("qtuser_3d.dll" in line):
                    tmp_2_array = qtuser3d_info_2_array
                elif("shader_entity.dll" in line):
                    tmp_2_array = shader_entity_info_2_array
                else :
                    tmp_2_array = other_info_2_array 
            elif(valid_info_pattern_end.search(line) or valid_info_pattern_end2.search(line) or valid_info_end3.search(line)):
                if(isValid):
                    outfile.write("\n")
                    for data in valid_data :
                        outfile.write(data)
                    if(tmp_2_array.count(valid_data) ==0):
                        tmp_2_array.append(valid_data)
                    valid_data = []
                    isValid = False

def func_write_module_out(output_path,func_module_info):
    with open(output_path, 'w', encoding='utf-8') as outfile:
        nIndex = 0
        for cur_array in func_module_info:
           nIndex +=1
           str_text = str(nIndex) + '\n'
           outfile.write(str_text)
           for info in cur_array:
            outfile.write(info) 

def func_write_json(filename):
    data = {}
    data['BasicKernel'] = basic_kernel_info_2_array
    data['qtuser_3d'] = qtuser3d_info_2_array
    data['shader_entity'] = shader_entity_info_2_array
    data['other'] = other_info_2_array
    # 写入json文件
    with open(filename, 'w') as file:
        json.dump(data, file, indent=4)  # indent=4 美化输出

if __name__ == "__main__":
    current_directory = os.path.dirname(os.path.abspath(__file__))
    parse_cache_dir = os.path.dirname(os.path.abspath(__file__)) + '/vld_result'
    if not os.path.exists(parse_cache_dir):
        # 如果目录不存在，则创建目录
        os.makedirs(parse_cache_dir)
    print(current_directory)
    input_file = current_directory + '/vld_cache.txt'  # 输入文件路径，包含要处理的文本
    output_file = current_directory + '/output_new.txt'  # 输出文件路径，保存过滤后的文本
    
    filter_valid_info(input_file, output_file)

    # for curArray in basic_kernel_info_2_array:
    #     print("a =",curArray)
    out_basic_kernel = parse_cache_dir + '/kernel_out.txt'
    func_write_module_out(out_basic_kernel,basic_kernel_info_2_array)
    
    out_basic_kernel = parse_cache_dir +  '/qtuser_3d_out.txt'
    func_write_module_out(out_basic_kernel,qtuser3d_info_2_array)

    out_basic_kernel  = parse_cache_dir +  '/shader_out.txt'
    func_write_module_out(out_basic_kernel,shader_entity_info_2_array)

    out_basic_kernel = parse_cache_dir +  '/other_out.txt'
    func_write_module_out(out_basic_kernel,other_info_2_array)

    out_basic_kernel = parse_cache_dir +  '/vld_result_parse.json'
    func_write_json(out_basic_kernel)
    print("有效信息已过滤并存储到", out_basic_kernel)
    
