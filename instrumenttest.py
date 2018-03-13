#-*- coding:utf-8 -*-

'''
RTMP 测试 demo
Created on 2017年2月27日
@author: fanniepeng
'''



import os
import sys
import subprocess
import plistlib
from xlwt import *
import xlrd
import xlsxwriter
import commands
class InstrumentsTestApi():
    '''
    RTMP 测试用例 demo
    '''


    def run_test(self,runtime,name):
        '''
        测试用例执行入口函数，将测试执行的操作和断言放在该函数内
        
        :param: none
        :return: none
        '''
        print 'step1:get deviceuuid'
        serial=subprocess.Popen("system_profiler SPUSBDataType | sed -n  -e '/iPad/,/Extra/p' -e '/iPhone/,/Extra/p' |  grep 'Serial Number:.*' | sed s#'.*Serial Number: '##",shell=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        deviceuuid=serial.stdout.read()
        serial.wait()
        deviceuuid= deviceuuid.split("\n")[0]     
        print deviceuuid
        tracename=name+'.trace'
        
        print 'step2:run instrument'
        tmp_cmd="instruments -t /Users/sherlock/Library/Application\ Support/Instruments/Templates/allin.tracetemplate -D "+tracename+" -w \"{"+deviceuuid+"}\" -l "+str(runtime)
        print tmp_cmd
        p2=subprocess.Popen(tmp_cmd,shell=True)
        p2.wait()


    def parserTraceDocument(self, file_path):
        '''
            运行traceparser返回解析结果
        '''
        # cmd1 = "chmod a+x %s" % self.parser_path
        parser_path = './traceDump'
        cmd1 = "chmod a+x %s" % parser_path
        commands.getoutput(cmd1)
        cmd = '%s %s' % (parser_path, file_path)
        _,ret = commands.getstatusoutput(cmd)
        return ret

    def exportanddrawexcel(self,result,filename='aa.xlsx'):
        print 'step 4:parse .trace file'
        wb=xlsxwriter.Workbook(filename)
        #deal with cpu data
        lines = result.split('\n')
        excelCol = ['B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N']

        #异常信息处理
        usefulList = []
        errorlist = []
        for line in lines:
            if 'Trace:'  in line or 'Device:'  in line or 'Process:'  in line or 'Target:'  in line or 'Instrument:'  in line or 'Run '  in line or 'time,'  in line or '#'  in line or line == '':
                usefulList.append(line)
        #delete error info from context
        # for line in errorlist:
        #     lines.remove(line)

        while not usefulList == []:
            row = 0
            contextList = []

            while not usefulList == [] and not usefulList[0] == '':
                contextList.append(usefulList[0])
                usefulList.remove(usefulList[0])
            if contextList == [] :
                usefulList.remove(usefulList[0])
                continue
            # print str(len(usefulList))+'--'+usefulList[1]
            # if  len(usefulList)>1 and  not usefulList[1] == '':
            #     continue

            try:
                sheetName = contextList[0].split(':')[1]
            except IndexError:
                sheetName = "null"

            ws = wb.add_worksheet(name=self.getInstrumentName(sheetName))
            for context in contextList:
                col = 0
                dataArr = context.split(', ')
                for data in dataArr:
                    if row > 2 and not ':' in data :
                        if 'MB' in data:
                            if ',' in data:
                                data = data.split(',')[0]+data.split(',')[1]
                            data = float(data.split(' ')[0])
                        elif 'KB' in data:
                            if ',' in data:
                                data = data.split(',')[0]+data.split(',')[1]
                            data = float(data.split(' ')[0])/1000
                        elif '%' in data:
                            data = float(data.split('%')[0])/100
                        try:
                            ws.write(row, col, float(data))
                        except Exception, e:
                            ws.write(row, col, data)
                    else:
                        ws.write(row, col, data)
                    col += 1
                row += 1
                # lines.remove(context)

            if row >4:
                chart = wb.add_chart({'type': 'line'})
                for c in range(col-1):
                    values = "="+self.getInstrumentName(sheetName)+"!$"+excelCol[c]+"$4:$"+excelCol[c]+"$"+str(row)
                    name = "="+self.getInstrumentName(sheetName)+"!$"+excelCol[c]+"$3"
                    chart.add_series({'values': values,'name':name})
                # chart.set_y_axis({'name': 'use'})
                chart.set_title({'name':self.getInstrumentName(sheetName)})
                ws.insert_chart(8, 15, chart)
            # if not lines == []:
            #     del(lines[0])
        wb.close()

    def getInstrumentName(self, str):
        if 'trace' in str:
            return 'TraceInfo'
        if 'CPU' in str:
            return 'CPU'
        elif 'Allocation' in str:
            return 'Allocations'
        elif 'GPU' in str:
            return 'GPU'
        elif 'Monitor' in str:
            return 'Activity_Monitor'
        elif 'Core Animation' in str:
            return 'Core_Animation'
        elif 'Energy' in str:
            return 'Energy'
        elif 'Network' in str:
            return 'Network'
        else:
            return None
    
        
if __name__=="__main__":
    # filePath = '/Users/sherlock/Desktop/AllCPU.trace'
    # fileName = 'allcpu'
    # runtime = 180000
    filePath = sys.argv[1]
    fileName = sys.argv[2]
    if os.path.isdir(filePath):
        print filePath
    else:
        print '.trace file does not exist'
    result=InstrumentsTestApi()
    parser_result=result.parserTraceDocument(filePath)
    result.exportanddrawexcel(parser_result,fileName+'.xlsx')
