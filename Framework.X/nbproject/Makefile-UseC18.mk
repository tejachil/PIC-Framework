#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-UseC18.mk)" "nbproject/Makefile-local-UseC18.mk"
include nbproject/Makefile-local-UseC18.mk
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=UseC18
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Framework.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Framework.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360937237/interrupts.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/messages.o ${OBJECTDIR}/_ext/1360937237/my_i2c.o ${OBJECTDIR}/_ext/1360937237/my_uart.o ${OBJECTDIR}/_ext/1360937237/timer0_thread.o ${OBJECTDIR}/_ext/1360937237/timer1_thread.o ${OBJECTDIR}/_ext/1360937237/uart_thread.o ${OBJECTDIR}/_ext/1360937237/user_interrupts.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360937237/interrupts.o.d ${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/_ext/1360937237/messages.o.d ${OBJECTDIR}/_ext/1360937237/my_i2c.o.d ${OBJECTDIR}/_ext/1360937237/my_uart.o.d ${OBJECTDIR}/_ext/1360937237/timer0_thread.o.d ${OBJECTDIR}/_ext/1360937237/timer1_thread.o.d ${OBJECTDIR}/_ext/1360937237/uart_thread.o.d ${OBJECTDIR}/_ext/1360937237/user_interrupts.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360937237/interrupts.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/messages.o ${OBJECTDIR}/_ext/1360937237/my_i2c.o ${OBJECTDIR}/_ext/1360937237/my_uart.o ${OBJECTDIR}/_ext/1360937237/timer0_thread.o ${OBJECTDIR}/_ext/1360937237/timer1_thread.o ${OBJECTDIR}/_ext/1360937237/uart_thread.o ${OBJECTDIR}/_ext/1360937237/user_interrupts.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-UseC18.mk dist/${CND_CONF}/${IMAGE_TYPE}/Framework.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F26J50
MP_PROCESSOR_OPTION_LD=18f26j50
MP_LINKER_DEBUG_OPTION= -u_DEBUGCODESTART=0xfe68 -u_DEBUGCODELEN=0x190
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1360937237/interrupts.o: ../src/interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/interrupts.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -ms -oa- -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -sa -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1360937237/interrupts.o   ../src/interrupts.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/interrupts.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -ms -oa- -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -sa -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1360937237/main.o   ../src/main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/messages.o: ../src/messages.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/messages.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -ms -oa- -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -sa -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1360937237/messages.o   ../src/messages.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/messages.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/messages.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/my_i2c.o: ../src/my_i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/my_i2c.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -ms -oa- -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -sa -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1360937237/my_i2c.o   ../src/my_i2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/my_i2c.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/my_i2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/my_uart.o: ../src/my_uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/my_uart.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -ms -oa- -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -sa -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1360937237/my_uart.o   ../src/my_uart.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/my_uart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/my_uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/timer0_thread.o: ../src/timer0_thread.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timer0_thread.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -ms -oa- -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -sa -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1360937237/timer0_thread.o   ../src/timer0_thread.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/timer0_thread.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/timer0_thread.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/timer1_thread.o: ../src/timer1_thread.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timer1_thread.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -ms -oa- -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -sa -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1360937237/timer1_thread.o   ../src/timer1_thread.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/timer1_thread.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/timer1_thread.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/uart_thread.o: ../src/uart_thread.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/uart_thread.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -ms -oa- -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -sa -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1360937237/uart_thread.o   ../src/uart_thread.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/uart_thread.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/uart_thread.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/user_interrupts.o: ../src/user_interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/user_interrupts.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -ms -oa- -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -sa -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1360937237/user_interrupts.o   ../src/user_interrupts.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/user_interrupts.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/user_interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/_ext/1360937237/interrupts.o: ../src/interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/interrupts.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa- -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -sa -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1360937237/interrupts.o   ../src/interrupts.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/interrupts.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa- -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -sa -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1360937237/main.o   ../src/main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/messages.o: ../src/messages.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/messages.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa- -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -sa -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1360937237/messages.o   ../src/messages.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/messages.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/messages.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/my_i2c.o: ../src/my_i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/my_i2c.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa- -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -sa -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1360937237/my_i2c.o   ../src/my_i2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/my_i2c.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/my_i2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/my_uart.o: ../src/my_uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/my_uart.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa- -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -sa -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1360937237/my_uart.o   ../src/my_uart.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/my_uart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/my_uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/timer0_thread.o: ../src/timer0_thread.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timer0_thread.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa- -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -sa -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1360937237/timer0_thread.o   ../src/timer0_thread.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/timer0_thread.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/timer0_thread.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/timer1_thread.o: ../src/timer1_thread.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timer1_thread.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa- -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -sa -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1360937237/timer1_thread.o   ../src/timer1_thread.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/timer1_thread.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/timer1_thread.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/uart_thread.o: ../src/uart_thread.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/uart_thread.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa- -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -sa -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1360937237/uart_thread.o   ../src/uart_thread.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/uart_thread.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/uart_thread.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/user_interrupts.o: ../src/user_interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/user_interrupts.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa- -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -sa -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1360937237/user_interrupts.o   ../src/user_interrupts.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/user_interrupts.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/user_interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Framework.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG -w -u_EXTENDEDMODE -g -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 -z__MPLAB_DEBUGGER_PICKIT2=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}/../lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/Framework.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
else
dist/${CND_CONF}/${IMAGE_TYPE}/Framework.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w  -w -u_EXTENDEDMODE -g -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}/../lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/Framework.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
	@echo Normalizing hex file
	@"/Applications/microchip/mplabx/mplab_ide.app/Contents/Resources/mplab_ide/mplab_ide/modules/../../bin/hexmate" --edf="/Applications/microchip/mplabx/mplab_ide.app/Contents/Resources/mplab_ide/mplab_ide/modules/../../bin/en_msgs.txt" dist/${CND_CONF}/${IMAGE_TYPE}/Framework.X.${IMAGE_TYPE}.hex -odist/${CND_CONF}/${IMAGE_TYPE}/Framework.X.${IMAGE_TYPE}.hex

endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/UseC18
	${RM} -r dist/UseC18

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
